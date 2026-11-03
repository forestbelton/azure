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

## The unit of work is a FAMILY

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

  `func_800BF1C8` is parked for exactly this, at 0 DIFF with .text identical in
  content and size. It is the unit's only gcc-emitted jump table so far, so the
  next one attempted is the next one to hit it. See
  docs/matching-levers.md, "cc1psx emits `.align 3` for jump tables".
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

Six of six matched on the first round with this brief. Two things in it are
load-bearing: pointing them at `csrc/ovl_town.c` for types stops them inventing
a parallel type vocabulary, and asking for a quantified near-miss rather than a
verdict stops the reports that say "this appears to require a device".

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

Both are excluded from `worklist.py`. `parkstats.py --check` and `nmcheck.py`
must stay at 0 problems; a park with no parseable `func_XXXXXXXX (unit)` header
line fails both, and because `parkstats.py` treats the FIRST comment block as
the header, running it on such a file REPLACES the prose with generated stats.
Keep the one-line header stub at the top of every park and the documentation
below it, in the body, where `normalise_body` preserves it.

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
