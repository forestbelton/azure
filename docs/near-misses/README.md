# Near-miss sources for the functions that have not landed

**Empty as of 2026-11-01: every function in the resident executable has
landed.** The procedure below is kept because it is the one that finished the
project, and because the overlays will need it again. `probe-cse-forwarding.c`
is not a park -- it is a standing compiler probe.

One file per function that is written but not matching: the source that got
closest, plus a generated stats header. They are **not** compiled into the
game -- the function is still an `INCLUDE_ASM` stub and `make check` neither
builds nor checks them.

## The header is generated; do not hand-edit it

    python3 tools/decomp/parkstats.py            # regenerate every header
    python3 tools/decomp/parkstats.py --check    # exit 1 if any is stale

Each header carries the word count, the DIFF, and `advise.py --terse`'s
aligned findings. Re-run `parkstats.py` after changing a park source,
`include/structs.h`, a prototype, or a unit's flags. `nmcheck.py` parses the
same first line, so it and `parkstats.py` agree by construction:

    python3 tools/decomp/nmcheck.py              # compiles every park, checks counts

DIFF counts words positionally, so a one-word shift inflates it. The
`advise.py` findings in the header are the differences that survive alignment;
`VERDICT: ALLOC` means every opcode, immediate and displacement already
matches.

An agent given one function to land should be pointed at
`docs/landing-a-function.md`, which is the whole procedure and is written to be
read instead of this file.

## Working on one

    cp docs/near-misses/func_X.c zz_f.c
    python3 tools/decomp/verify.py zz_f.c <unit> <func>     # --gas-G from the unit
    python3 tools/decomp/advise.py zz_f.c <unit> <func> --terse

For the `VERDICT: ALLOC` and shape-search cases, run the permuter instead of
sweeping shapes by hand -- see `docs/permuter-protocol.md`. `unname.py <func>`
deletes each local in turn and re-scores; re-run it after adopting a hit.

Set `AZURE_SCRATCH` to a private directory if anything else in the repo is
compiling at the same time, so the two runs do not overwrite each other's
`.s` files. (This used to need a private wine prefix as well; the native
compiler has no shared state.)

## Landing gate

In order, and do not skip to step 3:

1. `verify.py` prints `RESULT: MATCH` on the park file. Nothing else licenses
   touching `csrc/` -- not a build that links, not an agreeing word count, and
   not `nmcheck.py`, which never reads `csrc/`.
2. Find the stub by name: `grep -n 'func_XXXXXXXX' csrc/<unit>.c`. Replace
   that function's own `INCLUDE_ASM` line.
3. `make check`, then `make containers`. Read the hashes, not the exit status.
4. `tools/decomp/guard.py` -- prints `N known, N accounted for` and names
   anything missing.
5. Delete the park file, mark the `docs/carved-sweep.md` row, update the counts
   in `docs/where-we-are.md`.

A non-matching function in `csrc/` is worse than no change: `progress.py` drops
the whole unit from both numerator and denominator, which once moved the
headline from 84.6% to 82.7% and read as lost work.
