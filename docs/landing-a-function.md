# Landing one function

You have been given this document and a function name. `FUNC` below is that
name. Work only on it. Do not start other functions, do not reorganise the
project, do not update any project-wide document, and do not commit. Your
deliverable is the function and its park file; the counts, the sweep table and
the commit are the caller's.

Your scarce resource is context, not CPU. A permuter candidate costs ~25ms and
no tokens; a hypothesis you reason out costs a turn, and a turn costs the whole
conversation again. Start the search before you start thinking.

## 0. Setup and orientation

    make permuter-prefixes                        # once per machine; needed
    python3 tools/decomp/parkstats.py docs/near-misses/$FUNC.c

Always pass that path. Bare `parkstats.py`, and `parkstats.py --check`,
re-measure every park in `docs/near-misses/` -- two compiles each, all of them
irrelevant to you. One park is well under a second.

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

ONE SEARCH AT A TIME. The jobs are a machine-wide budget of 20, which the
user's own machine is inside -- `permrun.py` counts what every other running
permuter already holds, clamps `--jobs` to what is left, and refuses outright
once the budget is spent. Launch the search once and leave it: if it is still
running, that is the search, not a reason to start another. A relaunched
search does not replace the old one, it adds to it.

If you need to stop a search, say so in your report and let the caller do it;
do not work around a refused `pkill` by starting more.

DO NOT WAIT ON IT WITH `pgrep -f`. A pattern like `pgrep -f "permrun.py
$FUNC"` matches the command line of the very shell running it, so it always
reports a hit and a wait-loop built on it never fires -- and `pkill -f` on
the same pattern always returns a nonzero exit for the same reason, which is
NOT evidence that nothing was running. Read the exit code of either as "no
information". What to do instead: the run appends exactly one row to
`nonmatchings/permlog.tsv` when it finishes, so wait on that row appearing,
or bracket the pattern (`pgrep -f "[p]ermrun.py $FUNC"`) to stop it matching
itself.

`--jobs` must also not exceed the prefix pool; `permrun.py` refuses otherwise,
because concurrent compiles in one wine prefix swap outputs and manufacture
false winners -- so oversubscribing the budget is a correctness bug and not
just a slow machine.

## 2. While it runs, do the cheap mechanical checks

These cost CPU and almost no context:

    python3 tools/decomp/search.py docs/near-misses/$FUNC.c <unit> $FUNC
    python3 tools/decomp/unname.py $FUNC

`search.py` sweeps its transform set; `unname.py` deletes each local in turn
and re-scores. Re-run `unname.py` after adopting any hit, because each one
changes the base. If either improves on the base, adopt it, re-run
`parkstats.py` on the park file, and restart the permuter from the new base.

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
3. Work out what the mutation actually does. Permuter output is mutated C:
   dead tests on uninitialised locals, duplicated arms, redundant casts,
   pointless temporaries. Name the mechanism -- a basic-block boundary, a
   reference count, a different allocator seeing a value, a cse run boundary --
   because a device whose effect you cannot state is a coincidence you cannot
   maintain.
4. Try for a legal spelling with the same effect, and re-run `verify.py` after
   each attempt; each costs ~2s. If one holds the match, prefer it.
5. If none does, **land it as a device anyway** -- see below. The search for a
   legal spelling is cleanup, not a gate.

### Decomp devices

A device is a construct the original author would not have written that makes
the bytes come out right. They are allowed. The project already ships one:
`csrc/unk_11F0C.c:194` has `__asm__ volatile("" ::"r"(q) : "memory")`, worth 13
measured words, with a comment recording each job it does.

The reason this is not a compromise: `make check` is byte-identical, so the
emitted instructions *are* the original's. Undefined behaviour in a dead test
lives in the C abstract machine and never reaches the binary -- gcc emits no
code that reads those locals -- and this project is pinned to one compiler out
of necessity, so the portability argument that normally makes UB serious does
not apply here.

Conditions, all of them:

- `make check` and `make containers` byte-identical, as for any landing. That
  is the only thing a device is allowed to be justified by.
- Annotate it `/* DEVICE: ... */` so every one is greppable. Say what the
  construct is, what it is worth in words, and what the function does without
  it. The barrier in `unk_11F0C.c` predates the marker and does not carry it
  yet, so a census means grepping for both until it is retrofitted.
- Record the legal spellings you ruled out, with their scores, in the same
  comment. A later attempt starts where you stopped rather than re-measuring.
- If the device warns under `-Wall` -- an uninitialised read does -- say so in
  the comment. The default `CFLAGS` has no `-Wall`, so the build is silent, and
  the note is what stops somebody "fixing" it later.
- It must sit inside the function. A device that changes a shared header, a
  unit's flags or the splat spec affects other functions' output and is a
  different change needing its own measurement.

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
5. Delete the park file. Leave `docs/carved-sweep.md`, `docs/where-we-are.md`
   and `TODO.md` alone: their counts come from `progress.py` and `guard.py`,
   and whoever integrates your change runs those once for every function that
   landed rather than once per function.
6. If the landing established a new mechanism -- a measured one, not a
   hypothesis -- add it to `docs/matching-levers.md` as its own heading with
   the measurement, the same way step 5.4 asks for a park. A landing is the
   better occasion for it, not the worse one: the match confirms the mechanism
   instead of leaving it inferred from a floor. The spellings you ruled out and
   their scores belong at the site in `csrc/`, next to the code they explain.
   Do not run `leverindex.py`; the caller regenerates the index.

A non-matching function in `csrc/` is worse than no change: `progress.py` drops
the whole unit from both numerator and denominator, which once moved the
headline from 84.6% to 82.7% and read as lost work.

## 4b. Reading the compiler instead of guessing spellings

The permuter searches spellings. When the residue is small and structural --
a permutation, one extra or missing word, a register swap -- it is usually
cheaper to open the compiler and find the pass that makes the decision. The
`ovl_main_80020000` sweep of 2026-11-01 landed nine functions by hand with no
permuter at all, the later ones worked this way from the dumps; and in all
nine the park's MEASUREMENT TABLE was sound while its stated CAUSE was wrong.
Read a park's numbers as fact and its diagnosis as a guess.

The two stages are Windows binaries, so they go through wine -- `psyq/*.exe`
is not executable on its own, and `tools/ccpsx` drives the `ccpsx` DRIVER
rather than the stages. `WINEPATH` has to name `psyq/` or `cpppsx` fails with
"could not execute".

```sh
cd /tmp/.../scratch                      # dumps are written to the CWD
PROJ=/home/case/projects/azure
export WINEPREFIX=$HOME/.cache/azure-psyq WINEDEBUG=-all
export WINEPATH=$(winepath -w "$PROJ/psyq")

wine "$PROJ/psyq/cpppsx.exe" -I"$PROJ/include" -I"$PROJ/psyq/include" \
    -DPERMUTER f.c > f.i
wine "$PROJ/psyq/cc1psx.exe" -O2 -G0 -da -quiet f.i -o f.s
#                                 ^^^ -G0 for an overlay, -G8 resident
```

**The dumps land next to the `.i` file**, one per pass, in pass order:
`f.i.rtl`, `.jump`, `.cse`, `.loop`, `.flow`, `.combine`, `.regmove`, `.sched`,
`.lreg`, `.greg`, `.cse2`, `.jump2`, `.bp`, `.sched2`, `.shorten`, `.dbr`.
They only appear in wine's `Temp` as `PQ2.*` when `ccpsx` names the temporary
itself, so invoke the two stages yourself and read them in place. Verified
2026-11-01.

Which dump answers which question, as established so far:

| symptom | dump | what to look for |
|---|---|---|
| an invariant hoisted or not | `.loop` | `moved to N` vs `not desirable`, and the "N real insns" count the threshold test uses |
| two insns in the wrong order, same block | `.sched` | the ready lists; sched1 here schedules a block BACKWARD, so `T-1` is its LAST slot |
| a duplicated tail merged too far | `.jump` | how many insns the cross-jump absorbed |
| a value in the wrong register | `.lreg` / `.greg` | the quantity's reference count and birth/death |
| a constant that keeps or loses a copy | `.cse` | whether two constant pseudos were unified |
| something in a branch delay slot | `.dbr` | whether it was stolen from the jump's target |

Two cautions learned the hard way. A dump tells you what the pass DID, not
what it would do to different input, so a conclusion is worth having only once
you have changed the source and watched the dump change with it. And the
decisive pass is often earlier than the symptom: a delay-slot fill, a register
swap and a schedule permutation have all turned out to be consequences of a
`cse1` or `loop.c` decision several passes upstream.

The gcc 2.7.2 source this compiler descends from is unpacked at
`scratch/gcc-src`. Cite `file:line` for anything you conclude from it --
`loop.c`, `jump.c`, `sched.c`, `local-alloc.c`, `reorg.c` and `cse.c` are the
ones that have mattered.

## 5. If it does not match

The park file is the deliverable. Leave `csrc/` alone and:

1. Keep the best source you measured as the park body.
2. `python3 tools/decomp/parkstats.py docs/near-misses/$FUNC.c` to regenerate
   the stats.
3. Append to the header, under the generated block, only what you *measured*:
   the candidate count and floor from `permlog.tsv`, and each source spelling
   you tried with its score. Facts and numbers, no narrative, no verdict about
   what is reachable.
   A `/* DEVICE: */` marker goes at the SITE, in the body, next to the
   spelling it explains, with its measurements in the header. Park bodies keep
   their comments as of 2026-10-30; before that `parkstats.py` stripped every
   comment from the body on each run and markers left at the site silently
   vanished, which is why older parks carry theirs in the header.
4. If you established a new mechanism -- not a hypothesis, a measured one --
   add it to `docs/matching-levers.md` as its own heading with the measurement.
   That one is worth the edit, because nothing else records it. Do not run
   `leverindex.py`; the caller regenerates the index.

A quantified negative is a real result. An exhausted search written up as a
property of the function is not: this project has had four such conclusions
fall in one day.

## Hard rules

- Do not run `make splat`. It deletes `asm/nonmatchings` and re-splits a 26 MB
  container, and anything else reading those listings breaks.
- Do not touch `csrc/` before `RESULT: MATCH`.
- Do not commit. Integration is the caller's job.
- Do not run `parkstats.py --check`, `nmcheck.py`, `triage.py` or anything else
  that sweeps every park. You were given one function, and every tool here
  takes a path or a name.
- Do not edit a project-wide document. `docs/carved-sweep.md`,
  `docs/where-we-are.md` and `TODO.md` are bookkeeping for the caller.
  `docs/matching-levers.md` is NOT one of these: a new measured mechanism is
  wanted there whether the function landed or parked, as its own heading. Only
  the index is the caller's, so do not run `leverindex.py`.
- A device is fine; an unexplained device is not. If you cannot say what it
  changes, keep searching or park it with the measurement.
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
