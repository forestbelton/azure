# Near-miss sources for the parked functions

One file per parked function in `docs/open-matching-problems.md`. Each
reproduces the original's logic and is within a few instructions -- usually
within one register assignment -- of the reference bytes. They are kept here
because the reasoning in a commit message is not enough to resume from: the
next attempt needs the exact source that got that close, so it does not
re-derive it and re-rule-out the same shapes.

Three of the files here are not parked functions in that sense, and they are
the valuable ones to know about:

* **`gp-window-batch.c`** -- eleven functions that are clean MATCHes with exact
  word counts, not near-misses at all. They are blocked on a structural change
  (the `csrc/unk_14934.c` split), not on matching, and go straight into `csrc/`
  when it happens. Its header has the mechanism, the type findings and the
  declarations they need.
* **`func_80041588.c`** and **`func_80042BDC.c`** -- two of the three
  jump-table functions, at 164 of 164 words / 21 DIFF and 543 of 543 / 14
  DIFF. Also blocked on that split rather than on matching, and both carry the
  recovered case order for their tables, which exists nowhere else.

For those three, `verify.py` masks the relocated halfwords that reference a
jump table, so a MATCH from it is necessary and not sufficient -- only
`make check` is authority.

These are **not** compiled. They are not in `csrc/`, the corresponding
function is still an `INCLUDE_ASM` stub, and `make check` neither builds nor
checks them. Each one carries, in comments, what is still different and what
has already been ruled out.

To work on one, copy it to a `zz_*.c` scratch file in the project root
(gitignored) and run `tools/decomp/verify.py zz_foo.c <unit> <func>`; see
`docs/where-we-are.md` for the loop. Keep the diff count in the file's header
comment up to date so the next attempt starts from the current best.
