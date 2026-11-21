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

## Measure on a ONE-FUNCTION FILE, not on the unit

    python3 tools/decomp/parksplice.py FUNC /tmp/<yours>/ovl_dungeon.c
    python3 tools/decomp/onefunc.py FUNC --src /tmp/<yours>/ovl_dungeon.c \
            --out /tmp/<yours>/one
    python3 tools/decomp/aligndiff.py /tmp/<yours>/one/ovl_dungeon.c FUNC

**0.09 s a candidate instead of ~25 s.** That is the difference between
sweeping a park's whole source-order space and guessing at it: on 2026-11-20
it took `func_8008E700` through all 540 legal orders of its basin in a minute,
and `func_800B8FC8` through 320 insertion points for one device. It also makes
`tools/decomp/dump.sh` usable, so the `.lreg` refs/length numbers and the
`.sched` ready-list trace that most park arithmetic rests on are one command
away instead of impractical.

`onefunc.py` prints the one way the file can disagree with the unit -- a
callee DEFINED EARLIER in the unit with no prototype, which the real compile
sees declared and this file sees as an implicit `int f()`. Confirm the park's
stated residue on the standalone before sweeping, and re-measure the final
body against the real unit (and with `ovlverify` THERE, not on the standalone,
where its masked count includes the unresolved `jal` targets of everything
not in the file).

## PREFER A PLAIN-C DEVICE TO AN `asm` ONE

A residue that is an allocation or reference-count problem has two kinds of
cure, and they are not equally welcome in a landed body. `flow.c:2128` weights
each reference by `loop_depth`, so

    do
    {
        lvl = p[0x11];
    } while (0);

buys one more reference for no instruction -- and so does
`__asm__("" ::"r"(lvl))`, which is a scaffold. **Sweep the do-while first**,
along with a duplicated-arm `if (v) { s } else { s }` (its references are
counted by flow and merged by jump2's cross-jumping) and a surviving body
copy. Reach for the asm only when the plain sweep comes up short -- which
happens: `func_8009A8C0`'s best do-while is 16 rows against an asm pair's 0 --
and when it does, say so in the landed comment and give the plain scores you
rejected, so the next reader can see it was not laziness.

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

## Every park states its own DECLARATIONS, and `parkresidue.py` measures it

    python3 tools/decomp/parkresidue.py              # all parks, ranked by residue
    python3 tools/decomp/parkresidue.py func_X       # one

**A park body is not self-contained.** It needs externs, typedefs and callee
prototypes that live in the unit's top declaration block, and those were
historically reported only in the agent's hand-back message -- which is a
transcript, not the repo. On 2026-11-16 that meant **19 of 34 parks could not
be compiled from the repo alone**: the bodies were safe, the declarations
were not. Put them in the header, inside the comment:

    DECLARATIONS
    extern u8 D_800E0979[];
    void func_80067E2C(void *, void *);
    END DECLARATIONS

`parkresidue.py` splices the body into the CURRENT `csrc/ovl_dungeon.c`,
inserts that block, and runs `aligndiff` -- so the ranking is a measurement
rather than a number scraped from prose. **Do not rank parks by reading their
headers.** I did that once and told an agent a park was at 6 differing when
it was at 33.

A park that needs a PROTOTYPE CHANGE to an existing declaration cannot be
auto-measured, because the change has to replace a line rather than be added.
State it under the DECLARATIONS block as `PROTOTYPE CHANGE ON LANDING:` with
the callers you re-measured, and expect `parkresidue` to skip that park.

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

## WRITE THE `DECLARATIONS` MARKERS. THE FALLBACK CAN BREAK A SPLICE, NOT JUST MISS ONE.

`parkresidue.py` prefers an explicit block:

    DECLARATIONS
    extern u8 D_800DCFD0[];
    s16 func_X_s(s16, UnkVec *, u8 *, s16) __asm__("func_X");
    END DECLARATIONS

Without one it falls back to scraping declaration-SHAPED lines out of the
header comment, and on 2026-11-16 that fallback scraped a line of prose into
the unit and produced `unterminated character constant` **at a line number
inside the unit** -- a failure that looks nothing like a park problem.

The fallback is now stricter (a prototype's first token must look like a C
type, which rejects `It needs func_X(a, b);`) and a compile failure on that
path says so explicitly. **But it is still a heuristic over prose.** Write
the markers.
