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

## The header is hand-written, and it is the point

Unlike `docs/near-misses/`, nothing generates these headers. Each one should
carry: the word counts and the aligned difference count, exactly which
instructions differ, the mechanism if it is known, and **every spelling that
was measured and rejected, with its score**. That rejected-spelling list is
what stops the next reader paying for the same sweep twice.

## Do not leave a park for a landed function

When a function lands, delete its file here in the same change. A park for a
landed function has no listing to measure against and is pure confusion --
`docs/near-misses/README.md` records that failure mode from the resident
executable's sweep.
