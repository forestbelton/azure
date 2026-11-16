# Parked candidates for ovl_dungeon

One file per function: the best body found so far, with its residue written at
the top as a comment. **These are unit FRAGMENTS, not self-contained park
sources** -- each one is a function body that needs the ~1500 lines of
declarations at the top of `csrc/ovl_dungeon.c` to compile. That is why they
live here and not in `docs/near-misses/`, whose `parkstats.py` / `nmcheck.py`
contract assumes a park file compiles on its own.

Nothing here is built. The function is still an `INCLUDE_ASM` stub in
`csrc/ovl_dungeon.c` and `make check` neither compiles nor checks these files.

## Why this directory exists

On 2026-11-16 a routine cleanup wiped `/tmp/claude-1000`, which is where the
parallel sweep's agents had been told to keep their working files. It took
four functions that had been closed and never reported, five unit copies,
every saved candidate and the agents' own transcripts. Nothing in the repo was
lost. See `docs/dungeon-sweep.md`, "2026-11-16: /tmp IS NOT DURABLE".

So: **a candidate goes here the moment it is the best one, not at the end of a
run.** One file per function means concurrent agents never collide on a path.
Agents write the file; the dispatcher stages and commits it.

## Measuring one

    cp csrc/ovl_dungeon.c /tmp/<yours>/ovl_dungeon.c
    export AZURE_SCRATCH=/tmp/<yours>/sp
    # splice the body over this function's INCLUDE_ASM line, then:
    python3 tools/decomp/aligndiff.py /tmp/<yours>/ovl_dungeon.c func_XXXXXXXX
    python3 tools/decomp/ovlverify.py /tmp/<yours>/ovl_dungeon.c func_XXXXXXXX

`aligndiff` aligns the two streams before counting and is the honest number on
anything over ~200 words; `ovlverify` compares word N against word N, so one
transposed pair misses twice. A function is landed only at `0 differing` with
the word counts equal.

## Run `parkcheck.py` after editing a header

    python3 tools/decomp/parkcheck.py        # exit 1 if any park is malformed

Nothing compiles these files, so nothing catches a malformed one. The failure
is always the same: an **inner `/*` inside the header**, which in C ends the
outer comment at the first following `*/` and turns the rest of the header
into code. It goes unnoticed because every merge script extracts from the
DEFINITION LINE onward and never reads the header -- so the body measures
perfectly while the file as a whole is not valid C.

On 2026-11-16 three of thirty-five were malformed at once, two of them from
my own notes quoting `/* ... */` from compiler source. Write inner comment
markers as `(* ... *)` or drop them.

## The header is hand-written, and it is the point

Unlike `docs/near-misses/`, nothing generates these headers. Each one should
carry: the word counts and the aligned difference count, exactly which
instructions differ, the mechanism if it is known, and **every spelling that
was measured and rejected, with its score**. That rejected-spelling list is
what stops the next reader paying for the same sweep twice.

## If a candidate goes missing, try `nonmatchings/` first

`permimport.py` leaves a full copy of whatever it imported at
`nonmatchings/<func>/base.c`, plus a `source.c` with the permsafe hoists
applied. That directory is **gitignored but not temporary** -- it survives
anything short of a clean, including the `/tmp` wipe that started this
directory.

On 2026-11-16 `func_800C0404`'s body existed nowhere else: the agent that
wrote it was gone, `/tmp` had been cleared, and the only copy was the
permuter base from the previous day. It was recovered from there, re-measured
from scratch and is now in this directory. Check `nonmatchings/` before
concluding a candidate is lost.

(Each permuter output directory, `nonmatchings/<func>/output-NNN-M/source.c`,
is a full copy too -- but those are *mutated* sources and may contain
undefined behaviour or nonsense conditions the search invented. Read before
reusing; see `docs/matching-levers.md` on re-measuring permuter answers.)

## Do not leave a park for a landed function

When a function lands, delete its file here in the same change. A park for a
landed function has no listing to measure against and is pure confusion --
`docs/near-misses/README.md` records that failure mode from the resident
executable's sweep.
