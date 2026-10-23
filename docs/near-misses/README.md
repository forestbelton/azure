# Near-miss sources for the functions that have not landed

One file per function that is written but not matching. Each reproduces the
original's logic and is usually within one register assignment of the reference
bytes. They are kept here because the reasoning in a commit message is not
enough to resume from: the next attempt needs the exact source that got that
close, so it does not re-derive it and re-rule-out the same shapes.

These are **not** compiled into the game. The corresponding function is still an
`INCLUDE_ASM` stub and `make check` neither builds nor checks them. But they are
not unchecked either -- `python3 tools/decomp/nmcheck.py` compiles every file
here and compares the result against the count its own header claims, so a
header that has drifted, or a file that no longer compiles against the current
`include/structs.h`, is caught. Run it after any change to a header or to
`structs.h`.

To work on one, copy it to a `zz_*.c` scratch file in the project root
(gitignored) and run `tools/decomp/verify.py zz_foo.c <unit> <func>` -- with
`--g8` if the unit is in the Makefile's `MASPSXFLAGS_G8` list. See
`docs/where-we-are.md` for the queue and `docs/matching-levers.md` for the
levers. **Set `AZURE_WINEPREFIX` to a private copy of `~/.cache/azure-psyq`** if
anything else in the repo is compiling at the same time; concurrent `ccpsx` runs
share one prefix and silently lose an output.

`gp-window-batch.c` is the one file here that is not a near-miss: it holds
functions that were clean MATCHes blocked on a structural change rather than on
matching. **That change is done** -- see the size-record route in
`docs/addressing-and-translation-units.md` -- so the file is now a historical
record of how the `$gp` group was worked, and `nmcheck.py` marks it `multi`
because it holds several functions with no single count to check.

## Read the header, and then distrust its conclusion

Every file here records what has been measured, and that part has been reliable:
a word count with a shape next to it has not once turned out to be wrong. What
has repeatedly been wrong is the **conclusion** drawn from those measurements.
On 2026-10-23 four of these files were re-examined and four conclusions fell:

* **func_80041344** -- "2 DIFF, a deliberate price; splitting the index into two
  sets is what buys the allocation priority". It was not a price. The index was
  one statement in the original, and the two words were bought back for free by
  splitting a *different* statement.
* **func_80041588** -- "21 DIFF, one coupled three-register permutation", with
  the one named lever refuted in the file itself. It matched outright once the
  switch index, the flag word and the loop bounds were made one variable.
* **func_80042A80** -- "whatever the original wrote, the value reaching the
  extension was opaque to combine". The value was perfectly transparent and
  single-set; what mattered was that its *consumer* sat in the same basic block.
* **func_80042900** -- the right diagnosis of reorg's copy-vs-steal, with the
  mirror image of the working shape measured and the working one not.

The pattern is the same each time: a true mechanism is identified, the search
around it is exhausted, and the exhaustion is written up as a property of the
function. It is a property of the search. So when a header says a function is
unreachable, read the measurements as evidence and the verdict as a hypothesis,
and check whether any lever added to `docs/matching-levers.md` *since that
header was written* bears on it -- the git log of the header's own file will
date it.

All seven files now carry a numeric bound or a named pass check rather than an
exhausted search. That is a real improvement over "I swept and found nothing",
and it is still not what it can look like at a glance.

## "Unreachable from C" is never what it says

These bytes were produced by cc1psx compiling a C file. A C source that
reproduces them therefore exists -- not probably, not in principle: it existed,
and somebody wrote it. So every unreachability claim in these headers is
narrower than its wording, and the narrower version is the useful one:

> No source shape I measured produces these bytes, **with the compilation
> context held fixed.**

The fixed context is doing more work in that sentence than the measurements are.
It includes which functions share the translation unit and in what order, each
symbol's declared type and size, the `-G` value, the struct layout, and the
flags. Not one of those is evidence about the original source; all of them are
this project's inferences, several have been wrong before, and each of them
changes what the passes these headers blame actually see:

* the **unit boundary** decides which constants cse has already folded, what
  loop.c has hoisted, and which allocnos are live across a call -- so it can
  move a `preferences:` line or a conflict set wholesale;
* a **declared size** picks symbolic against split addressing at every site, so
  getting it wrong produces a register-shaped diff from a declaration-shaped
  cause;
* the **struct layout** can yield the right instruction with the wrong
  displacement, which also reads as a register problem.

A bound computed inside the wrong context is void, not weakened. So when a
header says a priority gap is five-fold and fixed by the target's own code, the
ratio is a measurement and "fixed" is a claim about a context that was assumed.
`docs/where-we-are.md` has the audit to run before sweeping source shapes
again; it is five questions and it is cheaper than another sweep.

Current bounds, each worth reopening either with an idea not on its list or with
a context question its own bound assumes away:

* `func_80041E70` -- one word; cse must put the constant in the equivalence
  class and `rtx_cost` must prefer it, and the only escape costs more than it
  saves. Bound assumes: the `li 16` and the body are in one cse run, which
  follows from the unit carve.
* `func_8004437C` -- two address halves owned by local-alloc and global-alloc,
  no spelling measured making them coincide. Bound assumes: this unit's carve
  and `D_80080AF4`'s declared size.
* `func_800407C0` -- a copy preference created by the memcpy call plus a 5x
  priority gap. Bound assumes: the call is in this unit and the parameters'
  pruned preferences follow from it.
* `func_80047054` -- one common cause, no shape measured splitting it. Bound
  assumes: the conflict set that follows from `p[0]` being local-allocated to
  `$a0` within the unit as carved.
