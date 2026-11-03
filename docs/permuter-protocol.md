# Running decomp-permuter on this project

The permuter searches source shapes by mutating and recompiling. A candidate
costs ~25ms of CPU and no context; a hypothesis reasoned out by hand costs a
turn, and a turn costs the whole conversation again. Measured on this project
2026-10-29: the marginal cost of a landing rose from 3.6M raw tokens per
function (2026-10-23, 501 landings) to 61M (2026-10-29, 118 landings). So the
search belongs in CPU.

Verified working 2026-10-29: `func_8004D880`, parked at 5 DIFF, scored 0 after
5,173 candidates in 2 minutes at 37.7 candidates/s, and `verify.py` confirmed
MATCH on the result.

## Setup, once

    git submodule update --init vendor/decomp-permuter vendor/old-gcc
    make cdk-gcc                      # the compiler, built in docker

Nothing else. This section used to require `make permuter-prefixes`, because
concurrent `ccpsx` runs in one wine prefix silently swapped each other's
output -- measured 3 of 80 at 8-way, each a *wrong answer that assembles* --
and a search selecting the minimum score turns that into false winners. The
native compiler is a plain process with no shared mutable state, so parallel
searches need no setup and `permrun.py` no longer caps `--jobs`.

Also needed: `~/.local/bin/mips-linux-gnu-{as,objdump}` shims, because the
permuter hardcodes that prefix while Fedora ships mips64 binutils.

## Per function

    python3 tools/decomp/permimport.py FUNC          # scaffolds nonmatchings/FUNC/
    python3 tools/decomp/permrun.py FUNC --minutes 60 --jobs 8

`permimport.py` writes `base.c`, `target.o`, `compile.sh`, `settings.toml` and
`function.txt` from the park source and the splat listing, at the unit's own
gas `-G`. It does not touch `csrc/` -- decomp-permuter's own `import.py` needs
the source to be a build input, which for a parked function means putting
non-matching code in `csrc/`.

`permrun.py` prints one line and appends it to `nonmatchings/permlog.tsv`:

    2026-10-29  func_8004D880  2m  5173  base=35  best=0  rate=37.7/s  found=output-0-1  jobs=8

`best < base` means it found something; the source is in
`nonmatchings/FUNC/output-*/source.c`. It refuses `--jobs N` larger than the
pool.

## What the permuter's output is, and is not

It is a *measurement*, not yet a landing. Its source is mutated C: the
`func_8004D880` winner reads two uninitialised locals in a dead `if (c || e)`
whose arms are identical, which costs zero words and creates the basic-block
boundary the listing needs.

Work out the mechanism, then try for a legal spelling with the same effect and
`verify.py` again. For that function three were measured and all three fold
away, leaving the park at 5 DIFF -- `goto L; L:`, `switch (0) { case 0: ... }`,
and an identical-arm `if (x)`/`else` (which emits the test: 30 DIFF). When
nothing legal holds the match, the shape lands as a documented device: see
"Decomp devices" in `docs/landing-a-function.md` for the conditions, and
`csrc/unk_11F0C.c:194` for the precedent.

`verify.py` then `make check` remain the only authority. A permuter score of 0
is worth running them on, nothing more: the scorer cannot see a jump table
landing at the wrong address.

## Reading a run that found nothing

Record it in the park header as a quantified negative -- candidates scored and
the floor -- which is worth more than a narrative sweep. The project's
recurring failure is writing up an exhausted search as a property of the
function; a candidate count is a fact about the search.
