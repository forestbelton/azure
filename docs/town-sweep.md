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

`tools/decomp/landedshapes.py` mechanises the part of this that can be:

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

1. `worklist.py ovl_town --count 6` for the next round.
2. For each item, either read it yourself or hand it to an agent (brief below).
   Agents are worth it for anything needing an individual reading — measured
   ~70K tokens for a function in the 50-80 word range, six in parallel.
3. **Re-measure every agent result yourself** before landing it:
   `python3 tools/decomp/ovlverify.py <file> <func>`. A handed-back report is
   model output; the measurement is the evidence.
4. Derive the siblings from `families.py --show`. Substitute the varying words
   and verify each sibling too.
5. Merge into `csrc/ovl_town.c`: replace each
   `INCLUDE_ASM("asm/nonmatchings/ovl_town", func_XXXX);` with the body, and add
   any new declarations. The declaration block is anchored on
   `s32 func_8003BD84(s32 x, s32 y);` — inserting before that line keeps the
   file's one ordering rule, which is that declarations precede all definitions.
6. `make build/town.bin`. **This is the gate.** It must print `OK byte-identical`.
7. Commit. Say what was measured, not what was hoped.

Scripted merges are worth it at this volume, but write them to the scratchpad,
assert every anchor occurs exactly once, and never let one run partially: a
failed assert should leave `csrc/` untouched so you can fix and re-run.

## VERIFY THE WHOLE UNIT, not just the function you landed

`ovlverify.py` takes MANY function names and compiles the file once, so
checking every landed function in the unit costs one compile:

    grep -oE '^[a-zA-Z_][a-zA-Z0-9_ ]*\**(func_[0-9A-Fa-f]{8})\(' csrc/ovl_town.c \
      | grep -oE 'func_[0-9A-Fa-f]{8}' | sort -u > /tmp/landed.txt
    python3 tools/decomp/ovlverify.py csrc/ovl_town.c $(cat /tmp/landed.txt | tr '\n' ' ') \
      | grep -v ' 0 differing'

Run it whenever `make build/town.bin` fails and the diff is in rodata or a jump
table, which is what a size change looks like from the outside. On 2026-11-02
the container came out +0x20 bytes with the only visible difference being
`func_800A4A58`'s switch table shifted by 0x18 -- the above found the real
cause in one command (`func_8008FFC8`: 152 words in the unit against 146
standalone) after three wrong guesses about which prototype had done it.

## The traps, each of which has cost something

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
   silently dropped list. Agents reported this count unprompted once asked for
   it and it caught nothing -- which is the point.
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

**There are SIX.** This section said "Both" until 2026-11-02 and listed only
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
