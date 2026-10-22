# Near-miss sources for the parked functions

One file per parked function in `docs/open-matching-problems.md`. Each
reproduces the original's logic and is within a few instructions -- usually
within one register assignment -- of the reference bytes. They are kept here
because the reasoning in a commit message is not enough to resume from: the
next attempt needs the exact source that got that close, so it does not
re-derive it and re-rule-out the same shapes.

These are **not** compiled. They are not in `csrc/`, the corresponding
function is still an `INCLUDE_ASM` stub, and `make check` neither builds nor
checks them. Each one carries, in comments, what is still different and what
has already been ruled out.

To work on one, copy it to a `zz_*.c` scratch file in the project root
(gitignored) and run `tools/decomp/verify.py zz_foo.c <unit> <func>`; see
`docs/where-we-are.md` for the loop. Keep the diff count in the file's header
comment up to date so the next attempt starts from the current best.
