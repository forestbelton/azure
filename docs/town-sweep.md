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

## The traps, each of which has cost something

* **`ovlverify` MASKS `jal` TARGETS.** It cannot see a wrong callee, so a
  function can measure 0 differing and still be wrong. When `families.py`
  shows a varying word that is a `jal`, that is a REAL call difference in the
  sibling, not relocation noise — substitute the callee too. This cost two
  bytes in the container on 2026-11-02 and is the one trap that survives a
  clean verify.
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

## The one park

`docs/near-misses/func_800BA00C.c` — a 279-word entity spawner, 558 words with
its twin, at 109 differing words with every difference a register assignment.
`worklist.py` excludes it. Read the park before touching it: it records what is
settled, a 142K-candidate permuter run that floored at 59 differing, and one
open question that should be settled from a `cc1psx -O2 -G0 -da` dump of
loop.c's decisions before any more CPU goes into it. Its three settled notes
have already been reused verbatim by an agent on a different function, so the
park has paid for itself whether or not it lands.
