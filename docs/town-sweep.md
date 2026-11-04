# Sweeping town.bin's overlay

How to continue decompiling `csrc/ovl_town.c` from a cold start. Written for a
session with no memory of the previous ones.

Numbers rot; the tools are the authority.

    python3 tools/decomp/worklist.py ovl_town --summary   # what is left, in bulk
    python3 tools/decomp/worklist.py ovl_town --count 6    # the next agent round
    python3 tools/decomp/progress.py                       # words landed
    python3 tools/decomp/guard.py                          # stubs accounted for

Read `docs/overlays.md` before writing any C. It carries the lever list, the
`-G0` facts, and the per-lever corrections this sweep has made. `docs/landing-a-function.md`
is the contract for one function.

## THE UNIT OF WORK IS NO LONGER A FAMILY -- IT IS A CLUSTER

**Read this before `worklist.py`'s ranking persuades you otherwise.** As of
2026-11-02 the family pool is drained: `worklist.py ovl_town --summary` reports
69 families of 2+ totalling 1,892 words against 986 twinless functions
totalling 49,009, and the largest family left is 48 words. The family-per-reading
economics below is still true and still worth taking when a family is offered,
but it can no longer be the plan.

What pays now is a CLUSTER: a set of functions sharing a caller, a callee, or a
record layout. Two measured examples from 2026-11-02, both landed in one
session:

* **The 0x800B2D00/0x800B4720 twin unit, 14 functions and 244 words, all
  fourteen 0 differing on the FIRST compile with no agent.** Two halves of one
  translation unit compiled twice over a different object layout: the record
  pointer at +0xA8 against +0x5C, the inner record's fields at +0x1C/+0x3C
  against +0x2C/+0x40. Substituting those four offsets turns any function in
  one half into its twin. `families.py` grouped five of the seven pairs; the
  other two, at 11 and 12 words, fall below `worklist.py`'s reporting cut and
  would never have been offered. They came out of the shared caller.
* **The 0x800B5xxx/0x800B6xxx panel and task cluster, 29 functions and over
  1,000 words.** Here the leverage compounds: each landed function's comment and
  each agent's report pinned the next one's arity, types and call-site shape, and
  three functions landed from a recipe handed over in another function's report
  without the dump being read at all.

So: pick with `worklist.py` (it excludes the parks), but GROUP THE BATCH BY
RELATEDNESS, NOT BY SIZE. The screen that costs one command and pays every time:

    # word count from the listing HEADER, not from counting lines
    grep -oE "nonmatching FUNC, 0x[0-9A-Fa-f]+" asm/nonmatchings/ovl_town/FUNC.s
    # jump table? rodata of its own? -- both are landing-blockers, see below
    grep -c 'jtbl_' asm/nonmatchings/ovl_town/FUNC.s
    sed -n '/^\.section \.rodata/,/^\.section \.text/p' asm/nonmatchings/ovl_town/FUNC.s | grep dlabel
    # the callee set, and which of them are already landed C
    python3 tools/decomp/ovlverify.py --dump FUNC | grep -oE 'jal +\S+'

A frontier where every callee is landed is worth more than a bigger function
whose callees are all stubs, because the landed callees carry their signatures
AND the comments explaining the family's mechanisms.

## The family economics, for when a family IS offered

`tools/decomp/families.py` groups the remaining stubs by instruction shape with

`tools/decomp/families.py` groups the remaining stubs by instruction shape with
`jal` targets and i-type immediates masked. Members of one family are the same
source with different constants, so **one reading lands all of them**. That is
the whole economics of this sweep: the ranking in `worklist.py` is total family
words, not function size, because a 3x25 family costs the same reading as one
60-word function and pays twice as much.

    python3 tools/decomp/families.py ovl_town --show func_XXXX

prints the shared shape once and then one row per member with only the words
that vary, resolved to the symbol or immediate each carries. That table is what
you write the generator from.

Roughly 300 of the remaining functions are in families of 2+; the other ~1000
have no twin and cost one reading each. Do the families first.

## A family's NEIGHBOURS are often near-siblings it does not group

`families.py` masks `jal` targets and i-type immediates to group, but it does
NOT mask the operator or the branch condition. So a function that is the same
source with `+=` for `-=`, or `> 0` for `< 0`, lands in no family at all and is
counted among the "no twin" singletons.

This is worth money. After landing `func_8008BACC` (33 words, family of 2), its
two listing neighbours `func_8008BB50` and `func_8008BA44` -- both reported as
singletons -- fell to the same body in one compile each: `+0x08` with `+=` and
`> 0` for the first, and `-0x100` plus one trailing store for the second. 67
words for two compiles and no reading.

So when a family lands, spend two minutes looking for a landed relative. TRY
THE CALLEE GREP FIRST -- it has been the productive one:

    # 1. a landed CALLER of one of your callees is often the same recipe
    python3 tools/decomp/ovlverify.py --dump func_XXXX | grep jal
    grep -n 'func_<a distinctive callee>' csrc/ovl_town.c

    # 2. only then the listing neighbours
    grep -n 'INCLUDE_ASM' csrc/ovl_town.c | grep -A3 -B3 func_XXXX

On func_800B260C both listing neighbours and every callee were still stubs, so
route 2 was a dead end -- and route 1, grepping `func_80053DA8`, found the
recipe 300 lines away in the landed `func_800ADD80`. The two are the SAME
SOURCE with a different callee set and one different constant, and it landed in
one compile. `landedshapes.py` cannot group such a pair, because the callee set
differs entirely; only the callee grep finds them.

Then try the landed body with the obvious operator or offset changed. The cost is one compile; `ovlverify` answers in seconds. This
matters more as the sweep goes on, because the family pool drains while the
~1000 singletons do not, and some unknown fraction of those singletons are
near-siblings of something already landed.

**`tools/decomp/nearsibs.py` MECHANISES THE SEARCH, and it is the first thing
to run on this unit now.** It compares the MNEMONIC STREAM of every stub
against every other stub of similar length and reports the pairs above a
similarity threshold -- which is precisely the thing `families.py` cannot see,
because masking `jal` targets and i-type immediates does not hide an operator,
a branch condition, a register allocation or a systematic field-offset swap.

    python3 tools/decomp/nearsibs.py ovl_town           # pairs, best first
    python3 tools/decomp/nearsibs.py ovl_town --groups  # transitive sets
    python3 tools/decomp/nearsibs.py ovl_town --ratio 0.88 --min-words 24

It found the four-function collision sweep at 0x8008DA80 -- **1,334 words,
landed from one reading plus substitution**, which `families.py` reported as
four separate "family of 1" entries -- and on 2026-11-02 it reported **18 sets
totalling 2,820 words** in this unit, the largest being
`func_800BA930`/`func_800BACB0` at 448. It takes seconds: it buckets by length
so the comparison never approaches O(n^2) over a thousand stubs.

A high ratio is a strong lead, not a promise. 1.000 means the same mnemonics in
the same order, which is what a twin looks like and also what two functions
built from one macro look like. Read both dumps before promising a word count,
and diff them with the register and immediate columns stripped -- that is what
shows the substitution:

    diff <(python3 tools/decomp/ovlverify.py --dump A | awk '{$1="";$2="";print}') \
         <(python3 tools/decomp/ovlverify.py --dump B | awk '{$1="";$2="";print}')

**AND GREP THE PARKS FOR A NEAR-SIBLING BEFORE YOU READ A DUMP.** It is one
command and it has been worth a whole reading:

    grep -l 'func_8003FD64' docs/near-misses/*.c    # a callee the pair shares

On 2026-11-02 `nearsibs.py`'s 0.964 pair `func_800BA930`/`func_800BACB0` turned
out to be the 224-word cut-down of the parked 279-word
`func_800BA00C`/`func_800BA468` pair -- same recipe minus a second random call,
a direction selector and a lean. The park's prose WAS the reading: its six
settled facts, its two varying constants (`0x1A`/`0xA` and `0x19`/`9`) and even
its `UnkTownRend` field list transferred verbatim, and the 448 words landed
without a permuter. The only two things the park got wrong for the shorter pair
were three statement orders and the number of renderable pointers.

A park is a written-up reading of a function nobody has landed yet. When a
`nearsibs.py` set shares a distinctive callee with one, read the park first.

`tools/decomp/landedshapes.py` mechanises the other half of this -- a stub
whose shape is ALREADY LANDED:

    python3 tools/decomp/landedshapes.py ovl_town            # shape match
    python3 tools/decomp/landedshapes.py ovl_town --exact    # raw-word match

It indexes every listing in the unit by opcode shape (registers and immediates
masked) and reports stubs whose shape is ALREADY LANDED, naming the function to
start from. `--exact` compares raw words with jal fields included, so a hit
there lands verbatim under a new name.

It found 5 stubs and 71 words on 2026-11-02, including func_8008C984, which is
the max-search mirror of the landed func_8008CA20 and needed only the seed
(-0x7FFF for 0x7FFF) and the comparison direction changed. It reports 0 once
those land. Expect it to be empty whenever families have been landed whole --
its value is as a check after a PARTIAL landing, and the number to watch is
the "mixed" count: a shape group holding both landed functions and stubs is
unfinished work the family tools cannot see.

## The loop

0. `nearsibs.py ovl_town --groups` FIRST, then `worklist.py ovl_town --count 6`.
   The near-sibling sets are worth more per reading than anything the worklist
   ranks, and the worklist cannot see them.
1. Pick a batch and **group it by relatedness, not by size** — see the cluster
   section above.
2. **Before writing any C, read the declaration block for every function and
   symbol you are about to name.** Four times on 2026-11-02 a signature that
   had been guessed was already declared there with landed callers, and each
   time it surfaced as a hard `conflicting types` error in the probe rather
   than as a byte difference. `grep -nE '^[a-zA-Z_].*func_XXXX\(' csrc/ovl_town.c`
   costs one call and the guess costs a compile and a rewrite. The same goes
   for the local convention: a neighbour four lines up often has the exact
   signature, and two of mine had to be corrected a commit later for ignoring
   one.
3. For each item, either read it yourself or hand it to an agent (brief below).
   Agents are worth it for anything needing an individual reading — measured
   77K to 420K tokens each over twenty-two runs on 2026-11-02, median ~110K,
   with the spread set almost entirely by the LEAD. Six in parallel do not
   contend, and the main session can land small families by hand meanwhile.
4. **Re-measure every agent result yourself** before landing it, IN A MERGED
   COPY of the unit and not only against its scratch file:
   `python3 tools/decomp/ovlverify.py csrc/ovl_town.c <func>`. A handed-back
   report is model output; the measurement is the evidence. Twenty-two reports
   on 2026-11-02 were all accurate, and the re-measurement still caught three
   signature CONFLICTS between agents working in parallel, each of which
   measured 0 differing in isolation.
5. Derive the siblings. Use `families.py --show` for a grouped family and the
   stripped-column dump diff above for a `nearsibs.py` pair. Substitute the
   varying words and verify each sibling too — and **read every varying SYMBOL
   out of its own member's listing**, never extrapolated from the base's.
6. Merge into `csrc/ovl_town.c`: replace each
   `INCLUDE_ASM("asm/nonmatchings/ovl_town", func_XXXX);` with the body, and add
   any new declarations. The declaration block is anchored on
   `s32 func_8003BD84(s32 x, s32 y);` — inserting before that line keeps the
   file's one ordering rule, which is that declarations precede all definitions.
7. `make build/town.bin`. **This is the gate.** It must print `OK byte-identical`.
   It is the ONLY thing that catches a wrong address-taken symbol, a rodata
   shift or a jump-table pad, all of which `ovlverify` masks.
8. Check the clang-format complaint set against HEAD:
   `clang-format --output-replacements-xml csrc/ovl_town.c | grep -c '<replacement '`.
   It was 37 all through 2026-11-02 and every rise was a real slip of mine —
   `void * c` for `void *c` once. Since the carve it is **31 for
   `csrc/ovl_town.c` alone and 36 across the three units** (31 + 0 + 5): count
   all three, because one complaint in the DUPLICATED preamble now costs 3.
9. Commit. Say what was measured, not what was hoped, and record the spellings
   that did NOT work with their numbers: a later session reads those to avoid
   re-deriving them, and three of today's landings came out of another
   function's rejected list.

Scripted merges are worth it at this volume, but write them to the scratchpad,
assert every anchor occurs exactly once, and never let one run partially: a
failed assert should leave `csrc/` untouched so you can fix and re-run.

## VERIFY THE WHOLE UNIT, not just the function you landed

`ovlverify.py` takes MANY function names and compiles the file once, so
checking every landed function in the unit costs one compile:

    # The DEFINITION list. Use awk, not a grep for a line ending in `)`:
    # 30 of this unit's definitions are written `void func_8008B2BC(void) {`
    # with the brace on the signature line, and a `)$` pattern silently drops
    # every one of them. On 2026-11-02 that under-reported a whole session's
    # sweeps as 762 of 762 when the real count was 792.
    # And exclude any line containing a `;` at all, not just one ending in it:
    # a declaration with a trailing comment -- `u16 func_80066460(s32, s32,
    # s32, s32); /* GetTPage */` -- does not end in `;` and otherwise gets
    # swept as a definition, reported as `not defined in csrc/ovl_town.c`.
    awk '/^[A-Za-z_].*func_[0-9A-Fa-f]{8}\(/ && !/;/' csrc/ovl_town.c \
      | grep -oE 'func_[0-9A-Fa-f]{8}' | sort -u > /tmp/landed.txt
    wc -l < /tmp/landed.txt            # POSITIVE CONTROL: compare against
                                       # progress.py's landed count for the unit
    # Expect ONE more than progress.py: a multi-line DECLARATION's first line
    # (`void func_800C3050(void *a, s32 id, void *a2, void *a3, void *a4,`)
    # has no `;` on it either. That name is still a stub, so it sweeps at 0
    # differing and costs nothing -- but know which off-by-one is benign, and
    # chase any larger gap.
    # PRINT THE VERIFIED COUNT, not just the failures.  `grep -v ' 0 differing'`
    # alone turns ANY hard exit into EMPTY OUTPUT, which is indistinguishable
    # from a clean sweep -- see "A sweep that verified nothing" below.  Always
    # compare `verified-clean` against progress.py's landed count.
    out=$(python3 tools/decomp/ovlverify.py csrc/ovl_town.c $(tr '\n' ' ' < /tmp/landed.txt) 2>&1)
    # Compare BOTH counts. `reported` against the number of names catches a
    # sweep that stopped early; `verified-clean` against progress.py catches a
    # sweep that ran but measured the wrong set.
    echo "names      $(wc -l < /tmp/landed.txt)"
    echo "reported   $(echo "$out" | grep -cE '^func_[0-9A-Fa-f]{8}:')"
    echo "clean      $(echo "$out" | grep -cE '^func_[0-9A-Fa-f]{8}: .* 0 differing')"
    echo "$out" | grep -E '^func_[0-9A-Fa-f]{8}:' | grep -v ' 0 differing'
    echo "$out" | grep -E 'NO TARGET|no listing|not defined|Traceback'

## A sweep that verified nothing, and how to tell

**On 2026-11-03 this sweep was verifying ONE function out of 618 and reporting
clean.** splat emits `asm/nonmatchings/<unit>/<func>.s` only for a function
still carried by `INCLUDE_ASM`, and PRUNES it the next time it runs -- so every
already-landed function loses its listing. `asm/nonmatchings/ovl_town*/` held
763 listings against 1,059 landed functions, `ovlverify.py` hard-exited on the
first name it could not resolve (`func_8008AC84`), and the recipe's closing
`grep -v ' 0 differing'` swallowed the exit message. Empty output. Several
sweeps reported as "clean" in this doc's own history were vacuous, including
ones taken as evidence after the carve.

Two things were wrong and both are fixed:

1. **`ovlverify.py` now falls back to the REFERENCE BINARY** when a listing is
   gone. It needs no listing at all: `build/<container>.elf` carries every
   function's address and size, and the vram->rom map is the section **LMA**
   (`.B000` is VMA 0x80088760, LMA 0xb000). Use the LMA, **not `File off`** --
   that is 0x28760 and reads the wrong bytes. Positive control, re-runnable:
   for all 763 functions that still had a listing, the reference-derived words
   equalled the listing's words exactly, 763 checked, 0 mismatches. The sweep
   then verified **1,060 names clean across the three units** -- the first
   non-vacuous whole-unit sweep since the pruning started.
2. **The recipe prints its own coverage**, so an empty failure list can no
   longer be read as success.

**THE SAME DEFECT RECURRED ONE LEVEL DOWN, on the same day.** An agent's sweep
reported `verified-clean=327` with an empty failure list -- a short, silent
run. The cause was not the pruning: `ovlverify` hard-exited in the MIDDLE of
the sweep with "`func_800A21EC` at 800A21EC is in no section of
build/town.bin.elf", because a PARALLEL `make build/town.bin` was rewriting
`build/town.bin.elf` at that instant and one address fell outside the
half-written file's sections. **A hard exit mid-sweep is indistinguishable
from a clean sweep once the failure grep runs**, which is exactly the shape
this section was written about.

Fixed: an unresolvable target is now reported as `FUNC: NO TARGET -- <reason>`
and the run CARRIES ON through the remaining names, so the count is honest;
and the message names the concurrent-build cause, since that is the one seen.
The recipe above now prints `names` / `reported` / `clean`, and `reported`
short of `names` is the tell that the run stopped early. **When another
session may be building, re-run the sweep after that build finishes rather
than trusting a number taken during it.**

The fallback is sound only while `make build/<container>` is byte-identical,
because the ELF supplies the layout -- which is the gate every landing already
passes, and if it ever fails, make fails first and loudly. One asymmetry: a
listing carries TEXT and `report()` uses it to decide which TARGET words carry
a relocation; the reference carries only words, so masking falls to the
candidate's own reloc table alone. That errs safely -- a word relocated in the
target but not in the candidate is a real difference and stays visible -- but
the printed table has no target-text column for a fallback function.

Run the sweep whenever `make build/town.bin` fails and the diff is in rodata or
a jump table, which is what a size change looks like from the outside. On 2026-11-02
the container came out +0x20 bytes with the only visible difference being
`func_800A4A58`'s switch table shifted by 0x18 -- the above found the real
cause in one command (`func_8008FFC8`: 152 words in the unit against 146
standalone) after three wrong guesses about which prototype had done it.

## The traps, each of which has cost something

* **TWO ADJACENT CALLS WITH IDENTICAL DELAY SLOTS ARE BYTE-INDISTINGUISHABLE
  TO `ovlverify`, SO THEIR ORDER IS ONLY CHECKABLE AGAINST THE RELOCATION
  LIST.** Measured 2026-11-03 on `func_800AF860` and `func_800AFFA8`: both are
  driver chains over the same `u8 *s`, both call `func_800AF674` and
  `func_800AF784` back to back with the same `addu $a0, $s0, $zero` in each
  delay slot, and SWAPPING THAT PAIR IS 0 DIFFERING IN BOTH. The `jal` word is
  masked and nothing else moves. The two functions' real orders are OPPOSITE --
  `func_800AF860` is AF520, **AF784, AF674**, AF254, AF858 while
  `func_800AFFA8` is AF448, AF520, **AF674, AF784**, AF254, AF858 -- so a brief
  that called them "the same driver chain" and let an agent copy one into the
  other would have shipped a wrong word with a clean verify. Check it like
  this, per function, and expect the two lists to match exactly:

      grep -oE '\bjal\s+func_[0-9A-Fa-f]+' asm/nonmatchings/<unit>/FUNC.s \
        | awk '{print $2}'
      python3 tools/decomp/relocdump.py CAND.c FUNC \
        | grep -oE 'R_MIPS_26\s+func_[0-9A-Fa-f]+' | awk '{print $2}'

* **A LANDED CALLEE'S PROTOTYPE CAN BE TOO NARROW FOR THE CALL SITE YOU ARE
  ABOUT TO WRITE, and when it is defined ABOVE you that is a COMPILE FAILURE,
  not a measurement.** `func_800AF858` was landed as `void func_800AF858(void)`
  -- correct for its own 2 words, which read nothing -- while both
  `func_800AF860` and `func_800AFFA8` set `$a0` AND `$a1` in its `jal`'s delay
  slot. Calling it with one argument is 1 differing in each caller; calling it
  with two against the old prototype is "too many arguments". The fix is to
  WIDEN the landed callee (unread parameters cost it nothing: it stayed 2 words
  at 0 differing) and that edit is outside the group's own INCLUDE_ASMs, so it
  has to be in the merge script and called out at hand-back. Re-measure the
  widened function AND its other callers, not just your own.

* **`ovlverify` MASKS EVERY RELOCATED WORD, NOT JUST `jal`.** A wrong
  ADDRESS-TAKEN symbol -- the `lui`/`addiu` pair of a handler install like
  `a->unk_50 = func_800C28E8;` -- is invisible to it, and invisible to a
  hand-decode of the `jal` words too, because there is no `jal`. On 2026-11-02
  a family-of-2 landing guessed `func_800C2938` for its sibling's handler, a
  symbol that does not exist; both members measured 0 differing and the
  container build failed with `undefined reference`. **Read the varying symbol
  out of the sibling's own listing** -- `grep -oE 'func_800C2[0-9A-F]{3}'
  asm/nonmatchings/ovl_town/func_800C2890.s` -- rather than extrapolating it
  from the base member's address. `families.py --show` prints it when it
  groups the pair; when it does not, the listing does.

* **`ovlverify` MASKS `jal` TARGETS.** It cannot see a wrong callee, so a
  function can measure 0 differing and still be wrong. When `families.py`
  shows a varying word that is a `jal`, that is a REAL call difference in the
  sibling, not relocation noise — substitute the callee too. This cost two
  bytes in the container on 2026-11-02 and is the one trap that survives a
  clean verify.
* **A PARAMETER THAT IS NEVER READ HAS NO WITNESS, so a 0-differing
  definition can still have the wrong ARITY.** `ovlverify` cannot see it and
  neither can `families.py`; only a caller can. On 2026-11-02 the
  `func_800B0158` family came back with one parameter, measured 0 differing on
  all four members, and the container build then failed with "too many
  arguments to function" because four landed callers pass two. An unused
  `void *` second parameter fixed it and all four still measured 0 differing,
  because an unused parameter costs no words. Check the arity BEFORE landing:
  `grep -n 'func_XXXX' csrc/<unit>.c` for landed callers, and when there are
  none, `grep -rl 'jal.*func_XXXX' asm/nonmatchings/ovl_town/` and read the
  caller's `$a0`/`$a1`/`$a2` setup. A function whose address is only TAKEN (a
  handler installed in a slot) has no arity witness at all, and any arity
  compiles.
* **AND THE DISPATCHER IS NOT A WITNESS EITHER, IN THIS UNIT.** It is tempting
  to read the arity of a handler off the call through its slot. Do not: the
  slots are declared `void (*unk_50)()` and `void (*unk_54)()` --
  UNPROTOTYPED -- and this unit calls them with THREE different argument counts
  at five different sites. `d->unk_50();` and `d->unk_54();` with **zero**
  arguments (`csrc/ovl_town.c:6409`, `:7690`, `:16109`, `:14765`),
  `a->unk_50(a, b, c)` with **three** (`:8232`), and
  `a->unk_50(a, a, q, r)` with **four** (`:7517`). Every one of those
  type-checks against every handler, so none of them constrains anything.
  **AND A REFERRER IS NOT A CALLER.** Before treating a name in
  `grep -rl 'func_XXXX' asm/nonmatchings/ovl_town/` as a call site, check
  WHICH: `grep -oE 'jal *func_XXXX|%hi\(func_XXXX\)'` on it. On 2026-11-02 I
  briefed an agent that `func_8009B6B4`'s three referrers were callers whose
  landing would settle its arity. All three only TAKE THE ADDRESS, there is no
  `jal` to either target anywhere under `asm/nonmatchings/ovl_town/`, and
  landing those three would add no witness at all.
  **The arity of a slot-installed handler comes from register usage in its OWN
  body**, and there are two usable forms: an argument register READ before it
  is written, and -- the sharper one -- a register the body never writes at all
  that a landed N-argument callee then reads. `func_80099C18` never touches
  `$a2` anywhere, yet calls a three-parameter `func_80098928`; that makes `$a2`
  incoming and the third parameter real, and dropping it is 51 words against 50
  with 43 differing. A slot assignment proves only WHICH SLOT the function
  occupies.
* **AN AGENT'S STANDALONE FILE CAN DECLARE A SYMBOL DIFFERENTLY THAN `csrc/`
  DOES, so its 0 differing does not transfer.** The agent writes its own
  `extern` for everything, and if the unit declares the same symbol as a
  STRUCT where the agent wrote `extern u8 x[]`, a call `f(x, ...)` passes the
  struct BY VALUE in the unit and a decayed pointer in the scratch file.
  `func_8008FFC8` measured 146 words standalone and 152 in the unit for exactly
  that reason, on `D_800CFCB4`, and the fix was `&D_800CFCB4`. So when a body
  mentions a symbol the unit already declares, CHECK THE UNIT'S DECLARATION
  against the agent's before merging -- and re-measure the function against
  `csrc/ovl_town.c` itself, not only against the scratch file, which is what
  the whole-unit sweep above does for free.

* **A MISSING ARGUMENT MOVE IS NOT EVIDENCE OF A ZERO-ARITY CALL.** gcc 2.7.2
  keeps the incoming `$a0` live across the prologue and reuses it when the
  first call's first argument is the function's own first parameter -- so
  `func_80033D08((u8 *)a)` compiles to a bare `jal` with a `nop` delay slot and
  no `move $a0, $sN`, even though `a` lives in a saved register throughout.
  One agent nearly talked itself out of a correct spelling over this. Read the
  CALLEE's own landed definition or listing for its arity, not the absence of a
  setup move at the call site.

* **`make build/town.bin` is the only authority.** `ovlverify` compares one
  function's words and is blind to a jump table landing at the wrong address
  and to rodata order.
* **Do not run `make splat`.** It re-splits the 26 MB dungeon container and
  clobbers shared listings. The listings under `asm/nonmatchings/ovl_town/`
  are already there.
* **Do not let an agent touch `csrc/`.** Six agents editing one 8000-line file
  is a merge conflict with extra steps. They write to `scratch/`; you merge.
* **Signature cascades are normal and the compiler finds them.** A parameter
  that is only FORWARDED carries no type information, and a function ending in
  a call leaves the callee's value in `$v0` whether or not it is declared to
  return one. So `s32` placeholders and `void` returns are guesses that only a
  CALLER can falsify. When one does, retype the function and let the build name
  every forwarder that now disagrees. This has happened six times; each time the
  bytes were unchanged, which is both why it is safe and why it could not have
  been known earlier.
* **Do not retype on one family's evidence.** There are ~160 `, s32 c)`
  signatures in the unit. `func_800C4174`'s third parameter is an
  `UnkTownRend *` because a family writes through it; `func_800C2E84`'s second
  probably is too, and it stays `s32` until something uses it.
* **SCREEN FOR A JUMP TABLE BEFORE YOU START.** If the listing has a `jtbl_`
  symbol, the switch has at least 5 cases and cc1psx emits a real table with
  `.align 3`. If the function ALSO owns a rodata constant, and the table's
  address is not 8-aligned, the body can reach 0 differing and the overlay will
  still fail with `size differs by +0x10` from four bytes of padding that no C
  spelling can remove:

      grep -l 'jtbl_' asm/nonmatchings/ovl_town/func_XXXX.s
      # table address 4 mod 8 AND a rodata constant of its own -> unlandable now

  **MEASURED AND NARROWED 2026-11-02: this threatens two stubs in the unit, not
  all of them.** `.align 3` only adds anything when the table's address is 4
  mod 8; at 0 mod 8 it emits nothing. Screening all seventeen jump-table stubs
  found FIFTEEN at 0 mod 8 and only `func_800BEBA4` and the parked
  `func_800BF1C8` at 4 mod 8 -- so func_800B7B8C (187 words), func_800B83DC
  (131), func_800B7F8C (121), func_800B8B00 (100) and func_800B7E78 (69) are
  ordinary work, and the parks func_800B85E8 and func_800B8E08 are searchable
  rather than blocked. The one-command screen is in
  docs/matching-levers.md under "cc1psx EMITS `.align 3` FOR JUMP TABLES";
  run it rather than skipping a function for having a table. Note too that
  func_800BF1C8's blocking constant is a LOCAL array initialiser that never
  appears as a `dlabel` in its listing, so grepping for one answers nothing --
  the alignment test is what to run.
* **A whole-unit 0-differing sweep does NOT mean the build passes.** ovlverify
  compares function TEXT. Rodata size and layout, jump-table padding and
  alignment are invisible to it. `make build/town.bin` is the only authority,
  and it is the step that catches a rodata regression three thousand words from
  anything you touched. The strongest check short of the build is an
  OBJECT-level byte comparison -- see docs/matching-levers.md, "a string
  constant's declared size decides whether .rodata survives", which has the
  exact commands. Use it whenever a function owns rodata, and under any hold
  that forbids writing to csrc/. It must use the SAME driver the build uses
  (`tools/cdkgcc`); a comparison built with a stale driver proves nothing.
* **TWO KNOWN-BENIGN RELOCATION-TABLE DIFFERENCES, and a count that goes DOWN
  is one of them.** An object-level comparison compares `.rel.text` record
  counts, and a landing legitimately changes them. Both classes seen so far
  are the stub-to-C conversion, not a defect:
  1. A **jump table** moving from a `jtbl_XXXXXXXX` symbol to a
     `.rodata`-section-relative reloc. The record count is unchanged; one
     `addiu %lo` word moves its addend from the relocation into the
     instruction field. Verify with `nm` that the symbol sits at the same
     `.rodata` offset.
  2. **Scratchpad addresses lose their relocations entirely.** splat renders a
     computed 0x1F800000 address as `addiu $a0, $s0, %lo(D_1F800078)` against
     an auto-named symbol; the C form writes the immediate and emits NO
     relocation. Measured 2026-11-03 on `func_800AD138`: baseline 14,409
     records against 14,397 merged -- **exactly 12 fewer**, all `R_MIPS_LO16`
     on auto-named scratchpad symbols, with `.text` and `.rodata` sizes
     identical.

  In both cases **the relocated words are masked, so a clean `ovlverify` run
  is a false positive on exactly these words** -- hand-decode each one against
  the listing. On `func_800AD138` all twelve were hand-checked identical. And
  note the asymmetry with the memory note on constant addresses: because the
  C form produces no `R_MIPS_LO16` where splat's `target.o` carries one,
  **decomp-permuter can never score zero on a function that touches the
  scratchpad.** Final bytes are the only authority there.
* **If the stub's listing has a `.section .rodata` block of its own, the C
  source must define that symbol, at its exact declared size.** splat migrates
  a constant into a function's own listing when it cannot attribute it
  elsewhere, so replacing the INCLUDE_ASM deletes it. `const u8 X[4] = "\nw";`
  works where `X[] = "\nw"` silently drops the trailing pad and shifts every
  later record by a byte, with .rodata's SIZE unchanged and the sweep clean.
* **UnkNode.unk_10 handlers are reached through func_80040190's cast**
  (csrc/unk_12DEC.c), which calls them as

      ((void (*)(void *, UnkVec *, u32)) fn)((u8 *) p + 0x20, p->unk_08, p->unk_0C)

  so THREE parameters, the first being `node + 0x20` -- the actor view, not the
  node. That is a reusable arity-and-offset witness for every remaining
  `n->unk_10 = (u32)func_XXXX;` stub (func_800903FC, func_8009DEBC,
  func_800B9260, func_800B90F0, ...), and it explains otherwise baffling
  `-0x2($s2)` / `0x20($s2)` accesses in such a body: `UnkNode.unk_1E` and the
  actor view's own fields off one pointer. The other +0x20 dispatcher,
  func_800402F4, goes through D_80083360[i] and is a DIFFERENT slot.
* **AN ARITY QUESTION HAS TO BE CHASED UP THE CHAIN.** The parameter trap
  above has a third form that cost two wrong landings on 2026-11-02. A function
  can STORE an argument register its body never writes -- `sw $a2, 0x0($v0)` --
  which means a forwarded parameter. Its caller forwards that argument into its
  own FIRST call, so the register is already correct at entry and gcc emits NO
  argument-setup insn: the caller measures identically with and without the
  parameter and cannot witness it in either direction. Only a call site that is
  not a pure forward can. `func_800B5174` and `func_800B5C60` were both landed
  at 0 differing with the wrong arity this way, and `func_800B6508` -- two
  levels out -- is what settled them. Two spellings that reach 0 differing
  without the parameter are both wrong and both were measured: a
  `register s32 c __asm__("$6")` device, and a bare uninitialised local, which
  is UB that merely happens to allocate to `$a2`. See
  docs/matching-levers.md, "A STORED ARGUMENT REGISTER THE PROTOTYPE DOES NOT
  HAVE IS A FORWARDED PARAMETER".

* **A NARROW PARAMETER'S WIDTH IS WITNESSED IN THE CALLER, AND THE BUILD WILL
  NOT CATCH IT.** When a parameter's only use is a store narrower than a word,
  `s16` and `s32` give identical callee bytes -- both 0 differing -- but an
  `s16` prototype costs a `sll`/`sra 16` pair per argument in EVERY caller.
  Three declarations in one batch were wrong this way on 2026-11-02, and on
  `func_800B691C` the wrong width in the function's own signature was worth two
  words plus an allocation swap three statements away (30 words / 15 differing
  against 29 / 0). Unlike an unread parameter this links and produces identical
  callee bytes, so `make build/town.bin` passes and only the caller's own match
  catches it. Read the caller: shift pair present means `s16`, bare `lw`/`sw`
  means `s32`.

* **DO NOT INVERT AN if/else UNLESS AN ARM RETURNS.** The "early `return` is
  laid out inline" lever reads like "a `beqz` to the second arm means write the
  inverted test first", and this sweep over-applied it. It holds only when one
  arm LEAVES the function. When both arms fall into a shared tail they are
  emitted in source order. Measured both ways: `func_800B5D30` (one arm
  returns) needs the inversion, 5 differing without it; `func_800B6508` (two
  plain calls) does not, and inverting it is wrong. Worse, inverting an
  `if/else` whose arms are calls swaps the `jal` targets too, and `ovlverify`
  masks those -- so a 3-word structural error reports as **1 differing** and
  reads like a near-miss worth polishing.
  And the arm must leave UNCONDITIONALLY: an arm spelled `if (x) return;` that
  then falls into the shared tail is a FALL-THROUGH arm for layout purposes.
  Measured on `func_800CD70C`/`func_800CD87C` -- writing that arm first is
  93 words against 92 with 35 differing, and 95 against 94 with 39.

* **NEVER `rm -rf build`, AND NEVER RUN `splat split --modes ld`.** Two
  operational traps, both of which cost time on 2026-11-03.
  `build/town.bin.ld` and `build/slus_006.14.ld` are GENERATED, their only
  rule is `build/%.ld: splat/%.yaml` -> `splat split $<`, and
  `expected/build/town.bin.ld` is a stale 459-byte copy that links to the
  wrong length. So deleting `build/` makes the overlay gate unrunnable until
  splat is re-run, which is the one thing the sweep forbids. To clean, delete
  `build/*.o` and `build/town.bin`, never the directory.
  And if you must regenerate only the linker script, `splat split
  splat/town.bin.yaml --modes ld` does produce it -- but it also **EMPTIES**
  `splat/town.bin_undefined_funcs_auto.txt` (187 lines) and
  `splat/town.bin_undefined_syms_auto.txt` (1039 lines), because the symbol
  collection pass is skipped. Both are tracked and both are linker inputs;
  `git checkout` them immediately afterwards and re-run both gates.
* **A PERMUTER NO LONGER NEEDS A PARK.** `permimport.py FUNC --source FILE`
  takes any file that defines the function and compiles -- a scratch candidate
  or a whole merged unit copy -- so a function that has never been parked can
  be permuted without inventing a park for it. Added 2026-11-02 after an agent
  hand-wrote all five scaffold files because its 44-of-46-word candidate had no
  park and `docs/` was forbidden to it. Without `--source` the behaviour is
  unchanged: `docs/near-misses/FUNC.c`. Output still lands in the gitignored
  `nonmatchings/FUNC/`, and `permrun.py FUNC --minutes N` drives it.
* **A CANDIDATE THAT IS SHORT CANNOT BE A cse-FORWARDING PROBLEM.**
  `func_800B683C` came in at 26 words against 28, 9 differing, with the target
  loading an address its own previous statement had stored to and `nop`s in
  both load-delay slots -- the documented signature of a volatile re-read.
  `volatile` changed nothing: the expression was one `*` short. Count the loads
  in the target's chain against the stars in yours before reaching for a
  device.

* **Do not spend a tool call on landedshapes.py for ovl_town.** It correctly
  reports 0 groups and will keep doing so while families are landed whole --
  which is what its own docstring says it is for. Two agents in one batch spent
  a call on it and got nothing. The productive routes, in order: the callee
  grep, then the CALLER grep, then families.py. The caller grep settled arity
  for three functions in that batch and gave a fourth its object layout.

### A FAMILY TEMPLATE CAN CROSS AN ARITY BOUNDARY -- CHECK THE DECLARATION FIRST

2026-11-03. `nearsibs.py` and `families.py` group on **opcode shape**, which is
blind to arity. The `func_80098928` four-member set was briefed with
`func_80098B9C` as its template; that is a **two**-argument
`func_80094984(desc, t)` form with `func_80099754` late, and all four targets
are **three**-argument with `func_80099754` first. The correct relatives,
`func_80098D60` and `func_80098DC0`, sit immediately after `func_80098CF0`'s own
`INCLUDE_ASM` and the brief never mentioned them.

The witness that settles it was already in the file: a full prototype at
`csrc/ovl_town.c:2322` plus **sixteen** landed three-argument call sites. So:
**before trusting a family template, read the unit's declaration for the target
and check the stub's immediate neighbours in the C file for a closer relative.**
A one-line `grep` beats the grouping.

### SWEEPING `nm` OUTPUT IS A FALSE-POSITIVE FACTORY

`INCLUDE_ASM` pulls every stub into the same object, so `nm` on a merged
object lists all 1819 names and **a stub verifies against itself**. Enumerate
the sweep from the **C definitions** (the `awk` recipe above), never from the
symbol table.

### THE TOOL INVENTORY -- AGENTS KEEP RE-DERIVING TOOLS THAT ALREADY EXIST

This doc and the generated brief named `provision.py`, `ovlverify.py` and
`advise.py` and nothing else, so agents re-built the rest by hand. Measured
cost: **six** separate agents hand-rolled a relocation dump, and one hand-rolled
the scheduler dump with `-dS` and reported it as a "recipe worth adding" --
`sched.py` had done exactly that for weeks. Both are now named in the brief's
HOW TO VERIFY. The full set under `tools/decomp/`:

| tool | what it answers |
|---|---|
| `provision.py UNIT FUNC...` | everything about a target's context: callees with position markers, installs, relatives (routes a/f/h/j), landed and ASM callers, address-takers, arity leads, parks in the call graph |
| `ovlverify.py FILE FUNC...` | word and differing counts, one compile for many names. **Masks every relocated word**; `--raw` leaves them |
| `relocdump.py FILE [FUNC...]` | the candidate's relocations at `-G0`, flagging the ones on a FUNCTION symbol -- the masked, build-breaking class |
| `sched.py FILE FUNC [--sched1]` | cc1psx's own scheduler dump: priorities, ready lists, hazard decisions, scheduled RTL; `--insn N` reads a number back |
| `aligndiff.py FILE FUNC` | shape-aligned diff printing EVERY row, where `advise.py` truncates at 12 |
| `advise.py FILE UNIT FUNC` | SHAPE/ALLOC/LENGTH verdict. **One function per file**, or it fabricates findings |
| `batcher.py UNIT` | partitions the remaining stubs into groups |
| `brief.py UNIT PREFIX FUNC...` | generates the agent brief; `--no-levers` drops the shape-matched block |
| `leverpick.py` / `leverpick_eval.py` | shape-matched lever selection, and its leave-one-out recall harness |
| `seqcheck.py FILE FUNC...` | decodes the words `ovlverify` MASKS: the ORDERED sequence of (jal symbol, `%hi`/`%lo` symbol, intra-function `j` offset) off the listing against the candidate's relocations, plus the `raw == relocdump total` arithmetic. Passes the constant-address subsequence case with a note; fails a symbol MISMATCH for hand decoding |
| `leverindex.py` | regenerates the lever index. **GENERATED -- never hand-edit; `--check` is read-only** |
| `parkstats.py` | regenerates each park's stats header; `--check` must stay clean |
| `nearsibs.py`, `worklist.py`, `progress.py`, `guard.py`, `magic.py`, `permimport.py`/`permrun.py` | the older seam-finding and permuter set |

**If you are about to write a script to answer a question about the compiler,
check this table first.**

### A SYMBOL THE LISTING SPELLS `D_` MAY BE A FUNCTION -- AND `D_` IS STILL THE ONLY SPELLING THAT LINKS

Hit on 2026-11-03 landing `func_8008B818`. Its listing takes the addresses of
`D_80033D54` and `D_80037714`. Both are **functions**: `csrc/unk_72A8.c:1044`
declares `void func_80033D54(void);` and `:1045` `void func_80037714(u8 *);`.

Writing the semantically correct `(u32)func_80037714` **compiles, measures 0
differing, and then fails the link with an undefined reference.** The reason is
which file defines the symbol for THIS overlay:

    splat/town.bin_undefined_syms_auto.txt:68   D_80033D54 = 0x80033D54;
    splat/town.bin_undefined_syms_auto.txt:70   D_80037714 = 0x80037714;

and neither `func_` name appears in `town.bin_undefined_funcs_auto.txt` or
`town.bin_undefined_syms_auto.txt` at all -- they exist only in the resident's
own yaml. So for an overlay, **the `D_` spelling is mandatory even when you can
prove the symbol is a function.** Note the trap's shape: the correct-looking
change is invisible to `ovlverify` AND to a clean whole-unit sweep, because
both compare text and the text is identical. Only `make build/town.bin` sees
it.

**Check the two `town.bin_undefined_*_auto.txt` files before renaming any
`D_` to a `func_`**, and say in a comment at the point of use why the ugly
spelling stays. (Those two files are also the ones `splat split --modes ld`
EMPTIES, which is why that command is forbidden.)

**And the same trap fires with a REAL LIBRARY NAME, which is worse, because
nothing warns.** Hit on 2026-11-03 landing `func_800951B4`. `provision.py`
route (c) prints, correctly, that splat names `func_800652AC` as
`OuterProduct0` -- and `OuterProduct0` is **already declared** by
`psyq/include/libgte.h:359`, which `include/common.h:39` includes. So
`OuterProduct0(&dir, &vel, &cp)` needs no declaration, produces no
diagnostic, and measures **107/107 at 0 differing** with an
`R_MIPS_26 OuterProduct0` relocation. It appears **0 times** in both
`town.bin_undefined_funcs_auto.txt` and `town.bin_undefined_syms_auto.txt`;
what this overlay links is `func_800652AC = 0x800652AC`
(`town.bin_undefined_funcs_auto.txt:155`), with `OuterProduct0 = 0x800652ac`
living only in `splat/slus_006.14_symbol_addrs.txt:107`. The useful rule:
**when a route prints a splat NAME, grep the overlay's two `*_auto.txt` files
and use the spelling that is in them**, whatever the psyq headers declare.

### A CANDIDATE MAY LEGITIMATELY RELOCATE AGAINST A DIFFERENT SYMBOL THAN THE LISTING NAMES

The ordered relocation-sequence check will flag it, and it is not always a
defect. Measured 2026-11-03 on `func_80098834`: splat's listing prints
`%hi(D_80082668)` / `%lo(D_80082668)`, while the landed candidate relocates
against **`D_80082660`** because it uses the unit's established
`D_80082660[1].unk_00 = 0;` idiom (~20 call sites) and the `+8` folds into the
instruction's displacement field -- `sb $zero, 8($v0)` with
`R_MIPS_LO16 D_80082660`.

Both resolve to the same words: `%hi` is `0x8008` either way, and the `%lo`
value is `0x2660 + 8 == 0x2668`, giving `3c028008` / `a0402668` -- exactly the
target's bytes. **So a symbol mismatch in the sequence check is a question,
not a verdict**: decode the resolved immediate by hand, and let
`make build/town.bin` settle it. Both symbols existed in
`town.bin_undefined_syms_auto.txt` (`:130` and `:131`), so either links; if
the one the candidate uses is NOT in those files, see the trap above.

### A CONSTANT ADDRESS MAKES THE CANDIDATE'S RELOCATION LIST A *SUBSEQUENCE* OF THE LISTING'S, AND THAT IS THE PASS CONDITION

A constant address emits no relocation at all, so splat's listing shows
`%hi`/`%lo` where the candidate has nothing, and the ordered
relocation-sequence check comes back with the candidate's sequence SHORTER
than the target's. Measured 2026-11-04 on the `func_8009F77C` group:
`func_8009F77C` has 6 masked words and 3 relocations, `func_8009F8EC` has 3
and 1.

**The raw comparison is what proves those words right.** `raw == relocdump's
total` means every word the candidate does NOT relocate matched byte for
byte -- including the ones splat renders as `%hi`/`%lo`. So the pass condition
is all three of:

1. the candidate's sequence is a SUBSEQUENCE of the target's,
2. every target-only entry is a `%hi`/`%lo` on a **DATA** symbol -- a missing
   one on a FUNCTION symbol is a build break, not this class,
3. `raw differing == relocdump's total`.

Report which words are constant-address and that condition 2 holds, rather
than reporting either a pass or a failure. And do not reach for
decomp-permuter on a function that uses a constant address: splat's `target.o`
carries an `R_MIPS_LO16` the constant form cannot produce, so the score never
reaches zero.

### A `%lo(D_...)` THE LISTING SHOWS MAY BE SPLAT MIS-SYMBOLISING A SMALL IMMEDIATE

Two in one function, found 2026-11-04 in `func_8009F77C`. `D_80011F80` and
`D_80010004` do not exist as source symbols: splat paired one
`lui $v1, 0x8001` with a `lbu` displacement, and paired the same `lui` with the
giv's `addiu $v1, $v1, 4`. **"`D_80010004`" as a symbol is the integer 4.**

These read exactly like declaration requirements and are not -- declaring
either would have prevented the landing. The tell is the one `provision.py`
already prints: several `%lo` off one `%hi` is the constant-base idiom. So when
a `%lo(D_...)` lands where a small immediate belongs, suspect the pairing
rather than the symbol.

### DO NOT DECLARE A STUB'S RETURN TYPE IF A CAST AT THE POINT OF USE MEASURES THE SAME

A declaration is an assertion about a function nobody has read yet, and a
later landing can contradict it. Measured 2026-11-03 in the `func_80096924`
landing: `u8 *func_80096A30();` and `(u8 *)func_80096A30(...)` on the implicit
declaration are both 67 words at 0 differing. The cast form was taken, with
the reason in a comment, because `func_80096A30` is still a stub -- the same
rule a brief already states for a callee's ARITY, applied to its return type.
A declaration that merely COPIES a landed definition's signature is a
different thing and is required when the symbol's address is taken.

### AN ARGUMENT-REGISTER MOVE AT ENTRY THAT NOTHING NEARBY READS IS A REAL PARAMETER

The converse of "A MISSING ARGUMENT MOVE IS NOT EVIDENCE OF A ZERO-ARITY CALL",
and it settles arity where no other route can.

`func_8008BC58` word 12 is `addu $a0, $a2, $zero`, sitting in the delay slot of
the function's FIRST branch, and nothing reads `$a0` again for forty words. It
reads as a dead copy. It is in fact the argument for two **bare**
`jal func_800478B8` at words 69 and 92, both with `nop` delay slots and no
`$a0` setup of their own: the parameter's pseudo is allocated to `$a0` and its
live range reaches both calls with no intervening call on either path.

That function has **no arity witness by any route `provision.py` checks** -- it
is installed at a handler slot, and the four landed steppers that take its
address leave their own third parameter unread. The entry move is the whole
witness, and it gives three parameters with `s32 c`.

**Fingerprint:** an `addu $aN, $aM, $zero` early, no nearby reader, and a later
`jal` with a `nop` delay slot and no argument setup. Look for the consumer
before concluding the move is dead.

### TWO AGENTS ON A CALLER/CALLEE PAIR WILL TYPE THE SAME RECORDS TWICE

Hit on 2026-11-03 with `func_800A3B80` and `func_800A41D8`, which are adjacent
in the image and are a caller/callee pair. Run in parallel, they landed an hour
apart, and the second merge **did not compile**: the caller's landing had added
a forward declaration

    u32 *func_800A41D8(UnkTownA3B80 *, UnkTownRend *, UnkTownA3B80SP *, u32 *, s16);

while the callee's own definition is

    POLY_FT4 *func_800A41D8(UnkTownA41D8 *, UnkTownA41D8B *, UnkTownA41D8SP *, POLY_FT4 *, s16);

Conflicting return type and three differently-named typedefs **for the same
three records** -- the callee's `UnkTownA41D8B` duplicates the existing
`UnkTownRend`, and its scratchpad typedef was the seventh to agree with
`Unk463ECSP` field for field.

**This is not a defect in either agent.** Neither can see the other's work, and
a pointer parameter's pointee type is unobservable from the function itself
(measured separately: three different pointee types, six measurements, all 0
differing), so each one necessarily invents a name.

**The resolution rule, in order:**
1. **Prefer the DEFINITION over the forward declaration.** A caller's forward
   declaration of a stub is almost always free -- the caller's own ablation
   table will usually already say so; this one measured 406/0 with the
   declaration omitted entirely. The definition is where the field accesses
   are, so it is the expensive one to change.
2. The unit rides an implicit declaration for a callee defined BELOW its call
   site **18 times**, so dropping the declaration is the established shape, not
   a compromise.
3. Re-measure BOTH functions after the resolution, not just the one you are
   landing. The gate that catches this is the pair, not the sweep.
4. Record the duplicate typing as cleanup rather than unifying it during a
   landing. Merging `Unk*SP` typedefs touches every function that uses them.

**And brief the second agent of a known pair** about what the first added: the
message that would have prevented this says which typedefs now exist and that
the forward declaration is a constraint it may contradict with numbers. I sent
exactly that message and it arrived after the agent had finished -- so send it
when you LAUNCH the pair, not when the first one lands.

### `grep -c dlabel` IS NOT A RODATA SCREEN -- `endlabel` CONTAINS `dlabel`

Every splat listing ends with `endlabel func_XXXXXXXX`, and the substring
`dlabel` sits inside the word `endlabel`. So an ad-hoc
`grep -c 'dlabel' asm/nonmatchings/ovl_town/func_XXXX.s` returns **1 on every
listing in the unit**, hazard or not, and screens nothing. Hit on 2026-11-03
pre-screening two 400-word stubs; it reported both as owning rodata when
neither does.

`brief.py` is not affected -- its test is `re.search(r'^dlabel D_', t, re.M)`,
anchored and typed. **Copy that form, or grep for `.section .rodata`**, which
has no such collision. More generally: when screening these listings, anchor
the pattern. `glabel`/`dlabel`/`endlabel`/`enddlabel` all share tails and
heads with each other.

### `advise.py` NEEDS ONE FUNCTION PER FILE -- AND IS ACTIVELY MISLEADING OTHERWISE

2026-11-03, hit independently by four agents in one batch. Given a merged
whole-unit copy it compares all 70,311 built words against the one function's
target and prints

    built 70311 words, target 66 words
    VERDICT: LENGTH
      LENGTH  built 70311, target 66 (+70245)
      CALLEE-SAVED  80091BD8  built saves $fp,$s4,$s5,$s6,$s7, target does not

The `CALLEE-SAVED` and `DELAY-NOPS` findings are **fabricated** -- artefacts of
the length mismatch -- and `--greg` printed a two-pseudo allocation table for a
function that has five. It is not merely unhelpful, it will send you after a
defect that does not exist.

One function per file, not one unit per file: a *two*-function solo file still
printed `built 111 words, target 55 words / VERDICT: LENGTH`. Build a minimal
one-function `.c` with its own `extern` declarations. `ovlverify.py` on the
merged copy is unaffected and remains the measurement.

### A BRIEF'S RODATA INVENTORY MUST NAME THE ADDRESS-TAKEN FUNCTIONS

A `%hi`/`%lo` pair on a *function* symbol -- a handler being installed into a
slot -- is masked by `ovlverify` and has **no `jal` to decode**, so it is
invisible to both of the usual checks. The `func_80092320` cluster's brief
omitted `%hi(func_8009204C)` and `%hi(func_80092CD4)` entirely, which is exactly
the case this doc records as having already cost one broken build. List them
with the data symbols, and read each one off the member's **own** listing.

### MY OWN BRIEFS WERE THE BATCH'S MAIN DEFECT SOURCE -- A CHECKLIST

In the 2026-11-03 batch of six clusters, every cluster landed and **every
cluster's agent found at least one error in its brief**. None was in the
analysis; all were in the provisioning. Before sending a brief, check:

1. **Is every function I call "landed in this unit" actually in this unit?**
   `func_80033B2C` was listed among "nine landed at 0 differing"; it is
   declaration-only at `:2211` with no listing in `asm/nonmatchings/ovl_town/`
   and lives in the resident.
2. **Does the named template have the target's arity?** See above.
3. **Do the callees I describe as "landed, read their prototypes" have
   prototypes that can serve the call sites?** `func_80094088`/`func_80094474`
   are defined with one parameter and called with three.
4. **Are my symbol counts per class, not totals?** `%hi(D_800837xx) x4` was
   really `%hi` x3 plus `%lo` x1.
5. **Is a source comment I am quoting about the function I think it is?** The
   block at `:4645` documents `func_800930E4`, the function defined *below* it.
   **This file's convention is that a comment block documents what follows.**
6. **Am I asserting a discriminator I have not measured?** The cast-placement
   "discriminator" was asserted twice, once as a mid-task correction, and does
   not exist.
7. **Does the inventory LIST the address-taken function symbols, not just warn
   about them?** 2026-11-03, second batch: a brief carried the `%hi`-on-a-
   function trap in prose but left `%hi/%lo(func_800A73E0)` and
   `%hi/%lo(func_800C8C3C)` out of the symbol inventory -- and those two installs
   were the entire point of both functions. The trap text tells the agent to go
   looking; the inventory hands it over. Run
   `grep -oE '%hi\(func_[0-9A-F]+\)' asm/nonmatchings/ovl_town/FUNC.s` per
   target and paste the result.
8. **Does the named template have the target's BODY, not just its arity?**
   Checklist item 2 asks about arity and that is not enough. A brief named
   `func_800CDCA8`/`func_800CDADC` as relatives for `func_800CD9F4`; they are the
   states it *transitions into*. One line --
   `grep -c 'func_800C30E0(a, b, c) == 0' csrc/ovl_town.c` -- finds seven real
   candidates including the two exact ones.

### THE RULE ABOVE WORKS: THREE BATCHES WITH PROSE, THREE ERRORS; ONE WITHOUT, NONE

2026-11-03, stated as a measurement rather than an exhortation. Across three
batches where I wrote "worth knowing in advance" paragraphs on top of
`provision.py`, **every brief contained at least one error**. In the batch where
the briefs carried only facts I had verified off the listings myself -- word
counts, no jump table, no own rodata -- and delegated everything else to the
tool, **all six agents reported no errors in their brief.**

What survived as useful non-tool content, and why it is different: a trap that
had already cost *me* something. One brief warned its agent not to transfer a
diagnosis from a neighbouring function with a matching differing count, because
I had done exactly that earlier in the sweep; the agent reported that the
caution "paid for itself" when the neighbour's recorded number turned out not to
generalise. So the line is not "say less" -- it is **say only what you measured
or what you got wrong**.

### A NEAR-MISS MUST SAY WHAT THE CANDIDATE HELD FIXED

Added after retiring a park that had 49 spellings and a permuter run floored on
its own plateau. The answer was a second lever interacting with a field hoist
the park held constant, and the park's own source reproduced its plateau
exactly -- so its measurements were sound and only its frame was wrong.

**A park is a statement about a search space, not about a function.** Every
near-miss should list the variables the candidate did not vary; without that, a
later reader cannot tell a hard wall from a corner of the grid.

### A HELPER COPIED OUT OF `scratch/` MAY BE COMPILING AT `-G8`

2026-11-03, and `scratch/` is gitignored so this warning has to live here.
Agents copy relocation/dump helpers from each other's `scratch/` files. One of
them called `asmlib.compile_asm(src)` **with no cflags, i.e. at the default
`-G8`**, while the overlays are built at `-G0`. It therefore dumped a *different
compilation* from the one `ovlverify` measures: one function rendered as **30
words with an assembler-expanded `lui $at`/`sw 0($at)` pair instead of 27 with
`lui $v1`**.

The symbol names stay correct, so the check such a script is used for does not
visibly fail -- it silently reports word counts and an instruction stream the
build never produces. **Any helper that compiles must pass
`cflags=('-G0',)`.** Check that before copying one, and before trusting a word
count that came out of one.

### RUN `provision.py`; DO NOT HAND-WRITE A PROVISIONING PARAGRAPH

2026-11-03. `tools/decomp/provision.py` computes a brief's whole provisioning
block: per-function callee lists, each callee classified LANDED-HERE /
STUB-HERE / NOT-IN-THIS-UNIT with its prototype (searching `include/` too) and
a marker when there is none, the `%hi`-on-a-function installs, relatives by
five routes, landed callers, ASM callers, address-takers, and an arity lead.

**Put its output in the brief, or tell the agent to run it. Do not paraphrase
it.** In the batch where I wrote "worth knowing in advance" paragraphs by hand
on top of the tool, my prose was wrong **three times in six briefs** and the
tool was right every time:

* I named `func_80095094` in a shared head where both targets call
  `func_800951B4`. **Substituting the wrong callee measures 177 words,
  0 DIFFERING**, because `ovlverify` masks the `jal` — only the relocation
  decode caught it, and the container would have been wrong by a word.
* I said two functions install `func_800A73E0`/`func_800C8C3C`. They install
  `func_800A7338`/`func_800C8B8C`; I had carried the names forward from the
  **previous batch's pair**. A wrong address-taken symbol is the one error
  class that survives a clean `ovlverify`.
* I swapped a pair's word counts.

The failure mode is always the same: a paragraph written from memory of the
last batch rather than from this batch's data. The tool has no memory.

### DO NOT TRUST A LOW RELATIVE SCORE, OR A TOOL'S "NOTHING FOUND"

Same batch, the other direction. `provision.py`'s rarity weighting is right to
discount a callee the whole unit calls, but it cannot see **position** (a
first-call entry point scores like any other) or **line-level identity** (what
makes a template is often a shared call expression *with its constant
arguments* — one global had 11 landed call sites but only 2 passed the same
`(x - 0x20)` form).

Twice the metric's bottom entries were the line-for-line template, and once I
briefed a group as having **no template** on a 0.156 top score — that 0.156 hit
*was* the template, and reading it anyway landed two functions on the first
compile. Two cheap complements found the right relative where the metric did
not, and both cost one command:

* the target's **immediate C-file neighbours**;
* an **IDIOM GREP** — grep a distinctive *statement* out of the listing rather
  than a callee name, e.g. `grep -n 'D_800814A0 |= 0x8000' csrc/ovl_town.c`.

### WHEN A PLATEAU IS BROAD, SUSPECT A VARIABLE YOU ARE HOLDING FIXED

The `func_800C1F8C` park recorded 49 spellings on a plateau at 15 differing and
a permuter run that floored on the base score. It is now landed, and the answer
was a **two-lever interaction**: a folding ternary abs *plus* removing a field
hoist the park held fixed. Each lever alone is neutral or worse, and the park's
own source reproduces its plateau exactly — the measurements were sound, only
the frame was wrong.

So a park is a statement about a search space, not about a function. Before
spending a permuter on one, list what the park holds constant and vary one of
those instead.

### NEVER DESCRIBE A GROUP'S CALLEE **UNION** AS SHARED -- RECOMPUTE THE INTERSECTION

2026-11-03, and it happened **twice in one batch** after I hand-edited a
machine-generated grouping. A clustering script printed, per group, the UNION of
its pairs' callees plus a `SHARED BY ALL` line computed over the pairs the
script itself had grouped. I then hand-expanded two groups with extra pairs and
**carried both lines across without recomputing them**. Results:

* "`func_80098928` and `func_800C4174` are shared by all four pairs" -- false.
  One pair called neither, and one lone function called neither.
* "All three pairs share the callee `func_8008F134`" -- false. One function's
  only two `jal`s were to something else entirely, so the two members of the
  pair did not even share the callee the group was named for.

Neither error cost a landing, but both sent an agent looking for a relationship
that was not there. **If you change the membership of a generated group, re-run
the generator -- do not edit its output.** And state per-function callee lists,
not per-group ones; a union reads as a shared set and there is no wording that
fixes that.

### THE PRODUCTIVE RELATIVE SCREEN IS ONE SHARED CALLEE **PLUS ONE SHARED GLOBAL**

2026-11-03. A two-or-more-shared-callees threshold reported "no landed function
shares two or more callees with these pairs, so there is no family template",
and that conclusion was wrong: `func_8009DFD8` (`csrc/ovl_town.c:8010`, landed)
is a line-for-line template for one of the pairs, which then landed in one
compile each. It shares exactly **one** callee plus **one** global
(`D_80083498`), so the threshold could not see it.

Screen on one shared callee plus one shared global. It costs one `grep`, e.g.
`grep -n 'func_8003FD64' csrc/ovl_town.c`.

### STATE SWEEP EXPECTATIONS AS A FORMULA, NOT A NUMBER

A brief that says "expect 879 of 879" is stale the moment any sibling group in
the same batch lands. In the 2026-11-03 batches `progress.py`'s count moved
879 -> 891 -> 905 -> 911 while agents were running, and two of them reported a
short sweep as a possible regression before working out it was my stale figure.
Write it as **"`progress.py`'s landed count, plus yours, plus the one benign
off-by-one"** and it stays true. Same for the lever count: cite
`leverindex.py`, not a number.

### RANK PROVISIONING BY CALLERS AND BY WHAT A FUNCTION *INSTALLS*, NOT ONLY BY CALLEES

2026-11-03, second batch. I rated six pairs excellent / moderate / thin on
shared-callee count and **the ranking came out backwards twice in one batch**:
both pairs I labelled "thin" were the best-provisioned, and both I labelled
"excellent" needed the most re-derivation. Shared callees are the wrong axis
for leaf code. Two cheaper routes, each one `sed` or `grep` away:

* **What the target INSTALLS.** `grep -oE '%hi\(func_[0-9A-F]+\)'` on the
  target's listing, then look up that function. `func_800A7360` and
  `func_800C8BB4` each install their own continuation, and both continuations
  were landed, adjacent in the file, and themselves a twin pair differing the
  same way -- one even carried a measured six-permutation store-order table
  that transferred verbatim and saved five compiles.
* **Landed CALLERS.** `func_800B44F0`/`func_800B4ED8` were rated thin because
  all four of their callees are resident, which predicted nothing. Their landed
  callers `func_800B4588` (`:11690`) and `func_800B4F60` (`:11919`) settle both
  arity *and* the second argument -- `func_800B44F0(v, 0x21)` and
  `func_800B4ED8(v, 0x29)` -- which no callee could have told me. And they are
  the fifth and sixth members of a landed family of four in the same unit.

### MEASURING THE CLANG-FORMAT COMPLAINT COUNT

Run it from the project directory so `.clang-format` is found, and **over all
THREE units**, because ovl_town was carved on 2026-11-03:

    for u in ovl_town ovl_town_800BEBA4 ovl_town_800BF3F0; do
      clang-format --dry-run csrc/$u.c 2>&1 | grep -c "warning:"
    done            # 31 + 0 + 5 = 36

**The total is 36, not the 37 it was before the carve**, and the arithmetic is
worth knowing because it is not a coincidence. The carve duplicates the
unit's 3,026-line preamble into each new unit, so any complaint IN THE PREAMBLE
is counted once per unit: 37 became 39 the moment the carve landed, all of it
one doubled blank line after `} UnkTown2AE8;`. Fixing that one line in all
three files -- the `MaxEmptyLinesToKeep` class, which has cost this sweep three
separate regressions -- took it to 36. So **a preamble complaint now costs 3,
and a body complaint costs 1.**

`--assume-filename` does **not** pick up the repo's `.clang-format` and reports
10,965; `-style=file:.clang-format` does. And `--dry-run -Werror` returns 0 for
that grep because `-Werror` changes the output text.

### THE UNIT IS NOW THREE UNITS, AND EVERY WHOLE-UNIT CHECK HAS TO LOOP

`ovl_town` was carved into `ovl_town` (to 0x800BEBA4), `ovl_town_800BEBA4`
(0x800BEBA4..0x800BF3F0) and `ovl_town_800BF3F0` (the rest). The sweep, the
format count and `progress.py` all report per unit now:

    for u in ovl_town ovl_town_800BEBA4 ovl_town_800BF3F0; do
      awk '/^[A-Za-z_].*func_[0-9A-Fa-f]{8}\(/ && !/;/' csrc/$u.c \
        | grep -oE 'func_[0-9A-Fa-f]{8}' | sort -u > /tmp/l_$u.txt
      python3 tools/decomp/ovlverify.py csrc/$u.c $(cat /tmp/l_$u.txt | tr '\n' ' ')
    done

**And the benign off-by-one is now THREE off-by-ones**, because the multi-line
declaration of `func_800C3050` lives in the duplicated preamble. Expect
`progress.py`'s per-unit landed counts plus one per unit.

## The agent brief

Copy verbatim, substituting the function name and its word count. Agents must
NOT inherit context — spawn fresh ones.

> You are decompiling one MIPS function in the Azure Dreams decompilation
> project at /home/case/projects/azure. Work only on `FUNC`.
>
> GOAL: write C that compiles to byte-identical MIPS for `FUNC` (N words) in
> the `ovl_town` unit, and report it.
>
> READ FIRST (in this order):
> 1. `docs/landing-a-function.md` — the contract for landing one function.
> 2. `docs/overlays.md` — especially the lever list ("Levers overlay code keeps
>    needing" and "Nine more levers"). The overlays are built at
>    `cc1psx -O2 -G0`, NOT the -G8 the resident executable uses.
> 3. `csrc/ovl_town.c` — READ ONLY. It already contains the struct typedefs
>    (UnkTownActor, UnkTown2AE8, UnkTownTask, UnkTownRend, UnkTownSpawn,
>    UnkTownArg80, ...) and hundreds of function declarations for this overlay.
>    REUSE those types and declarations rather than inventing new ones;
>    UnkTown2AE8 in particular is six 16.16 fixed-point words whose integer
>    halves are read as `*(s16 *)((u8 *)b + 0xA)` style casts.
>
> TOOLS:
> - `python3 tools/decomp/ovlverify.py --dump FUNC` prints the target instructions.
> - `python3 tools/decomp/ovlverify.py <yourfile.c> FUNC` compiles your C and
>   reports "N differing". 0 differing is the goal.
> - `python3 tools/decomp/families.py ovl_town --show FUNC` prints the shared
>   shape and which words differ between this function and its siblings —
>   useful because only the varying words are per-member.
> - `tools/decomp/magic.py <constant>` turns a division magic number back into
>   its divisor.
>
> IMPORTANT — SIGNATURE ARITY: a parameter that is never read has no witness in
> the listing, so a definition that measures 0 differing can still have the
> wrong number of parameters, and only the container build will catch it.
> Before you settle on a signature, `grep -n 'FUNC' csrc/ovl_town.c` and check
> how any EXISTING CALLER calls it and how many arguments it passes. If there
> are none, `grep -rl 'jal.*FUNC' asm/nonmatchings/ovl_town/` and read the
> caller's argument setup. Report what you found, including "no callers
> anywhere" if that is the case, and say which parameters your body actually
> reads.
>
> WRITE your candidate to `scratch/agent_FUNC.c` as a standalone file:
> `#include "common.h"`, then copies of whatever typedefs/declarations you need
> (taken from csrc/ovl_town.c), then the function. Nothing else.
>
> HARD CONSTRAINTS:
> - Do NOT edit `csrc/ovl_town.c` or any file outside `scratch/`.
> - Do NOT run `make splat` (it re-splits a 26 MB container and would clobber
>   shared state).
> - Do NOT run `make` or `make check`; the caller does the container build.
> - Do NOT commit anything.
>
> REPORT BACK: the complete final contents of your .c file, the final
> `ovlverify` line, and — if you did not reach 0 differing — exactly which words
> still differ, what you established about the cause, and what you ruled out. A
> quantified near-miss is a useful result; a narrative is not. Do not claim a
> match you did not measure.

### What is load-bearing in it, measured

Six of six matched on the first round with the brief above. On 2026-11-02
twelve of twelve matched across two batches, eight of them on the first
compile, and two agents returned a SECOND landed function they were not asked
for. The additions that produced that, in descending order of what they were
worth:

1. **A LANDED RELATIVE, named with its line number.** This is the single
   highest-value thing in a brief and the difference is an order of magnitude:
   measured, an agent handed one spent 74K tokens, one that found its own 93K,
   one reading cold 294K. `func_800B5774` landed on the FIRST compile purely
   from its sibling's body and comment. Quote the relative's body in the brief
   and point at the comment; do not just name it.
2. **THE WORD COUNT READ OFF THE LISTING HEADER, and the arithmetic shown.**
   `nonmatching FUNC, 0x1F8` is 126 words. Three of four briefs in one session
   had it wrong because the author counted lines instead. Write
   "**N words** -- its header reads `nonmatching FUNC, 0xHH`, and 0xHH/4 = N".
3. **THE HAZARD SCREEN, DONE FOR THEM.** State explicitly that the listing has
   no `jtbl_` and no `.section .rodata` block of its own -- or, if it has one,
   quote the block, say whether the symbol has its own `.s` file, and give the
   exact declared size. Agents do not reliably run this check and both hazards
   are silent until the container build.
4. **A UNIQUE SCRATCH FILENAME, stated as mandatory.** Subagents share the
   scratchpad and have clobbered each other's merged copies. Name the file.
5. **VERIFY IN A MERGED COPY OF THE UNIT, then sweep the whole unit in that
   copy** -- with the exact commands, and a POSITIVE CONTROL. Ask for both the
   `grep -v ' 0 differing'` output (which must be empty) and the
   `grep -c ' 0 differing'` count, so an empty result is distinguishable from a
   silently dropped list. **And check the count against `progress.py`'s landed
   figure for the unit**, because that is what catches the list being too
   SHORT. On 2026-11-02 the brief handed out a `grep` for a signature line
   ending in `)`, which silently drops the 30 definitions in this unit written
   `void func_8008B2BC(void) {` with the brace on the signature line -- every
   sweep that session reported "762 of 762" when the real count was 792. An
   agent spotted it and swept both lists. Nothing was broken, because
   `make build/town.bin` covers every function either way, but the claim was
   narrower than it sounded. Use the `awk` form above.
6. **"Decode the jal words yourself"**, with the formula
   `0x80000000 | ((w & 0x3FFFFFF) << 2)` and the reminder that a NESTED call is
   evaluated before its enclosing one. `ovlverify` masks the targets.
7. **A TABLE OF EVERY SPELLING MEASURED, demanded explicitly.** One agent
   returned a 28-point grid, another an 18-row table; both tables contained
   levers worth more than the landing. Asking for a quantified near-miss
   rather than a verdict is what stops the reports that say "this appears to
   require a device".
8. **The arity cautions, all three forms** -- missing move, present move,
   pure forward -- because the pure-forward form cost two wrong landings in
   one batch. See the traps above.

Two things to keep OUT: do not tell them to run `landedshapes.py` (it reports
zero for this unit and two agents wasted a call on it), and do not assert a
signature as "already settled" unless a non-forwarding call site witnesses it.
A brief that said "signature already settled, do not re-derive" was wrong
twice on 2026-11-02, and one agent correctly ignored it.

**Re-measure every result yourself before landing it.** Twelve of twelve
agent reports were accurate on 2026-11-02, and the re-measurement still caught
two signature CONFLICTS between agents working in parallel -- `func_800B5918`
reported as `s8 *` by one and `u8 *` by another, `func_800B6580` as `s16` by
one and `s32` by another. Both pairs measured 0 differing in isolation; only
the merge could choose.

## What is already established

All of it lives in `csrc/ovl_town.c`, in comments at the point of use. The ones
that come up constantly:

* `UnkTown2AE8` is six 16.16 words: a position at +0x00/+0x04/+0x08 and the
  per-frame delta for the same axis 0xC higher, at +0x0C/+0x10/+0x14. The
  integer half of each is the `s16` two bytes in, reached by a cast, because a
  named field cannot coexist with the word it lives inside. Four independent
  witnesses.
* `UnkTownActor` is the state-machine object: handlers at +0x50/+0x54, a
  stride-8 index at +0x60, step/wrap counters at +0x68/+0x6A, countdowns at
  +0x6C and +0x90, a PSX-angle facing at +0x72 (0x1000 to the turn), descriptor
  pointers at +0x7C/+0x80, and a centre/half-extent pair per axis in
  +0x84..+0x8E.
* `UnkTownTask` is the same idea one layer out, with handlers at +0x00/+0x04 and
  its countdown at +0x0A.
* The handler convention is `(actor, mover, rend)`, and most third parameters
  are still typed `s32` because nothing has used them yet.
* `D_80082660` is the engine's stride-8 record array; a constant index folds
  onto the symbol, so `D_80082660[4].unk_00 = 0` is how the `+0x20` slot is
  written, not a separate symbol.

## The parks

**There are SEVEN.** This section said "Both" until 2026-11-02 and listed only
the last two below; `ls docs/near-misses/` is the authority, and `worklist.py`
excludes all of them.

    python3 tools/decomp/parkstats.py --check    # must stay at 0 problems
    python3 tools/decomp/nmcheck.py              # the same

Ordered by what they are worth attempting next:

* **`func_800BF1C8.c` -- 138 of 138 words, 0 DIFF, and TOOLCHAIN-BLOCKED.**
  The .text is identical in content and size. It is parked because the function
  owns a rodata constant AND a gcc-emitted jump table whose address is 4 mod 8,
  so `.align 3` adds four bytes of padding no C spelling can remove and the
  overlay fails on `size differs by +0x10`. This is the unit's only gcc-emitted
  jump table so far, so the next one attempted is the next one to hit it. See
  docs/matching-levers.md, "cc1psx EMITS `.align 3` FOR JUMP TABLES". Nothing
  to search; it needs the container's layout to change.
* **`func_800B8E08.c` -- 139 of 139 words, 5 DIFF, verdict SHAPE, and it has a
  jump table.** Three of the five are an `addiu`/`lui` pair plus a MISSING and
  an EXTRA around 0x800B8FD8-0x800B8FEC -- one address materialisation in the
  wrong place -- and two are register-only. Flagged as permuter work and never
  given a permuter run. Screen the jump table's alignment before spending CPU:
  if it lands 4 mod 8 and the function also owns rodata, it is
  `func_800BF1C8`'s problem and no search can reach it.
* **`func_800B85E8.c` -- 148 built against 147 target, 94 DIFF, verdict
  LENGTH.** One word too long, with two COMMUTED `addu` operand orders at
  0x800B8618/0x800B867C and an OPCODE difference at 0x800B8724. Flagged as
  permuter work and never given one. Read the LENGTH line first: the +1 is the
  thing to fix, and the 94 is positional noise downstream of it.
* **`func_800B83DC.c` -- 131 of 131 words, verdict ALLOC, TWO register-only
  differences, and the closest park in the unit.** Parked 2026-11-02 after
  about 63 measured spellings. Two of its three switch arms are word-exact
  INCLUDING every register; the residue is that the target puts a `high` pseudo
  and its load's destination in one register where the candidate gives the
  `high` the lowest free one. The cause is read off the `.lreg`/`.greg` dumps
  and written into the park: the hard `$v0` set by the arm's `return 1` overlaps
  the load's live range but not the `high`'s, sched1 cannot reorder because the
  dependence is an anti-dependence with cost 0, and local-alloc's `combine_regs`
  never sees the pair because its operand loop strips `PLUS` and `MULT` but not
  `LO_SUM`. **This is the one to give the permuter first** -- it is the ALLOC
  class the protocol is for and the search space is one `if` statement.
* **`func_800B6FB8.c` -- 76 of 77 words, verdict LENGTH (-1), and the
  diagnosis is a compiler fact rather than a spelling.** Parked 2026-11-02
  after ~2,900 compiles. The target materialises the constant 7 TWICE for two
  adjacent halfword stores while parking its other two constants in one
  register each, and cse unifies the pair under every spelling reachable from
  C. Everything else in the function is pinned: a two-pointer variant reaches
  77 words with the wrong instruction and is byte-exact from word 38 to 76, so
  the search space is four lines. **func_800B72B8 (23 words, still a stub) has
  the same residue and the same cause** -- the same six halfwords off the same
  base, stuck at 22 of 23 -- so whatever lands one lands both. The park records
  what is ruled out and why, including a correction to the `do { } while (0)`
  lever, and the next move is the permuter, which has never been run on it.
* **`func_800BA00C.c`** and **`func_800CE2D4.c`** -- described below; both have
  real diagnoses already written down.

`parkstats.py --check` and `nmcheck.py` must stay at 0 problems; a park with no
parseable `func_XXXXXXXX (unit)` header line fails both, and because
`parkstats.py` treats the FIRST comment block as the header, running it on such
a file REPLACES the prose with generated stats. **Keep the one-line header stub
at the top of every park and put all prose BELOW it, in the body, where
`normalise_body` preserves it.**

The two with written-up diagnoses:

`docs/near-misses/func_800CE2D4.c` — 46 words, 92 with its byte-identical twin
func_800CE62C, at 13 DIFF. It deliberately carries the 13-DIFF spelling rather
than an 8-DIFF one that is structurally wrong; the park explains why, says
plainly that the generated verdict is SHAPE and not ALLOC, and names the
local-alloc `qty_order` decision a permuter run should aim at. No permuter has
been run on it.

`docs/near-misses/func_800BA00C.c` — a 279-word entity spawner, 558 words with
its twin, at 109 differing words with every difference a register assignment.
`worklist.py` excludes it. Read the park before touching it: it records what is
settled, a 142K-candidate permuter run that floored at 59 differing, and one
open question that should be settled from a `cc1psx -O2 -G0 -da` dump of
loop.c's decisions before any more CPU goes into it. Its three settled notes
have already been reused verbatim by an agent on a different function, so the
park has paid for itself whether or not it lands.
