# Landing one function

You have been given this document and a function name. `FUNC` below is that
name. Work only on it. Do not start other functions, do not reorganise the
project, and do not commit.

Your scarce resource is context, not CPU. A permuter candidate costs ~25ms and
no tokens; a hypothesis you reason out costs a turn, and a turn costs the whole
conversation again. Start the search before you start thinking.

## 0. Setup and orientation

    make permuter-prefixes                        # once per machine; needed
    python3 tools/decomp/parkstats.py docs/near-misses/$FUNC.c

The park header now holds the word count, the DIFF and `advise.py`'s aligned
findings. That header is the whole brief. **Read it and stop reading.** Do not
read the git log of the park, do not read `docs/matching-levers.md` end to end,
and do not run any diff with `--full`: the full side-by-side of a 400-word
function is ~10k tokens and you pay it again every turn afterwards.

`VERDICT` in the header tells you the class:

- `ALLOC` -- every opcode, immediate and displacement already matches; only
  register numbers differ. Nothing about the logic is wrong.
- `SHAPE` -- opcodes, immediates or displacements differ. The C is wrong.
- `LENGTH` -- the word counts differ; the header names the branch span the
  shift sits in.

If there is no park file for `FUNC`, you are writing the first draft: generate
it with `tools/m2c` (it takes `ctx.c`; regenerate with `make ctx` if a header
changed), read `docs/m2c-pitfalls.md`, get it compiling, then continue here.

## 1. Start the search first

    python3 tools/decomp/permimport.py $FUNC
    python3 tools/decomp/permrun.py $FUNC --minutes 60 --jobs 8 &

Launch it in the background and let it run while you do step 2. It prints one
line when it finishes. Do not poll it, do not tail its output, and do not read
`nonmatchings/$FUNC/base.c` -- it is the whole preprocessed translation unit.

`--jobs` must not exceed the prefix pool; `permrun.py` refuses otherwise,
because concurrent compiles in one wine prefix swap outputs and manufacture
false winners.

## 2. While it runs, do the cheap mechanical checks

These cost CPU and almost no context:

    python3 tools/decomp/search.py docs/near-misses/$FUNC.c <unit> $FUNC
    python3 tools/decomp/unname.py $FUNC

`search.py` sweeps its transform set; `unname.py` deletes each local in turn
and re-scores. Re-run `unname.py` after adopting any hit, because each one
changes the base. If either improves on the base, adopt it, re-run
`parkstats.py`, and restart the permuter from the new base.

Then audit the compilation context, because a bound computed in the wrong
context is void rather than weakened: which functions share the translation
unit and in what order, each symbol's declared type and size, the unit's `-G`
value, the struct layout. A wrong declared size produces a register-shaped diff
from a declaration-shaped cause.

Only if a specific mechanism is already named in the header should you open
`docs/matching-levers.md` -- read its index (the first ~150 lines), then only
the one section you need.

## 3. If the permuter improved on the base

1. Read `nonmatchings/$FUNC/output-*/source.c`. Read the function body only.
2. Confirm it: copy the body onto the park's includes and declarations as
   `zz_$FUNC.c`, then
   `python3 tools/decomp/verify.py zz_$FUNC.c <unit> $FUNC`.
   A permuter score of 0 is a lead; `verify.py` is the measurement.
3. Clean it up. Permuter output is mutated C and often illegal -- dead tests on
   uninitialised locals, duplicated arms, redundant casts. Work out what the
   mutation actually does (a block boundary, a reference count, a different
   allocator seeing a value) and find a legal spelling with the same effect.
   Re-run `verify.py` after each attempt; each one costs ~2s.
4. If no legal spelling holds the match, say so explicitly, record the
   spellings you measured and their scores in the park header, and stop. Do not
   land illegal C.

## 4. The landing gate

In order. Do not skip to step 3.

1. `verify.py` prints `RESULT: MATCH` on the park file. Nothing else licenses
   touching `csrc/` -- not a build that links, not an agreeing word count, and
   not `nmcheck.py`, which never reads `csrc/`.
2. Find the stub by name: `grep -n "$FUNC" csrc/<unit>.c`, and replace *that*
   function's own `INCLUDE_ASM` line. Replacing a neighbour's leaves one
   function defined twice and another not at all.
3. `make check > /tmp/check.log 2>&1; echo rc=$?` then `make containers`. Read
   the hashes, not the exit status, and never pipe make through `head`/`tail`.
4. `python3 tools/decomp/guard.py` -- it prints `N known, N accounted for` and
   names anything missing.
5. Delete the park file, mark the row in `docs/carved-sweep.md`, and update the
   counts in `docs/where-we-are.md`.

A non-matching function in `csrc/` is worse than no change: `progress.py` drops
the whole unit from both numerator and denominator, which once moved the
headline from 84.6% to 82.7% and read as lost work.

## 5. If it does not match

The park file is the deliverable. Leave `csrc/` alone and:

1. Keep the best source you measured as the park body.
2. `python3 tools/decomp/parkstats.py docs/near-misses/$FUNC.c` to regenerate
   the stats.
3. Append to the header, under the generated block, only what you *measured*:
   the candidate count and floor from `permlog.tsv`, and each source spelling
   you tried with its score. Facts and numbers, no narrative, no verdict about
   what is reachable.
4. If you established a new mechanism -- not a hypothesis, a measured one --
   add it to `docs/matching-levers.md` as its own heading with the measurement,
   then run `python3 tools/decomp/leverindex.py`.

A quantified negative is a real result. An exhausted search written up as a
property of the function is not: this project has had four such conclusions
fall in one day.

## Hard rules

- Do not run `make splat`. It deletes `asm/nonmatchings` and re-splits a 26 MB
  container, and anything else reading those listings breaks.
- Do not touch `csrc/` before `RESULT: MATCH`.
- Do not commit. Integration is the caller's job.
- Do not use `--full`, `--greg` or `dump.sh` unless the header's findings have
  run out; they are large.
- Name scratch files so they differ by more than case: wine's filesystem is
  case-insensitive and `ccpsx` will write the wrong one and exit 0.
- Every tool shelling out to `tools/ccpsx` needs `-Ipsyq/include` as well as
  `-Iinclude`.

## Report back

Four lines, no transcript:

- the function, its unit, and the before/after word count and DIFF;
- what landed, or what the floor is and how many candidates were scored;
- any source spelling that improved it, in one sentence of mechanism;
- what you changed on disk.
