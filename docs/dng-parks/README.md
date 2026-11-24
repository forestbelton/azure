# Parked candidates for DUNGEON.BIN's 116 carved objects

One file per function, named for the function: `func_b01_80172628.c`. The
names carry their unit's tag, so no two agents ever write the same path and
the directory needs no per-unit subdivision.

These are unit FRAGMENTS, not self-contained park sources: a body here needs
its unit's declaration block to compile. State those declarations in the
header, between `DECLARATIONS` and `END DECLARATIONS` markers, in the form
`docs/dungeon-parks/README.md` describes -- that convention is shared and the
reasons behind it are written up there.

Nothing here is built. The function is still an `INCLUDE_ASM` stub in
`csrc/ovl_dng_*.c`.

## A park here is usually a park for a WHOLE FAMILY

1199 of this block's 2575 stubs are exact copies of a sibling
(`tools/decomp/dngfam.py`). Before parking, run

    python3 tools/decomp/dngfam.py FUNC

and put the copy count in the header. A residue on a 46-copy representative
is worth 46 times what the word count says, and that is what decides whether
it is worth another day.

### AND A PARK IS A PARK FOR THE FAMILY, SO CHECK FOR ONE BEFORE WRITING ONE

`func_s20_80026010` and `func_s21_80025BA0` are one family -- the same 170-word
body -- and they are parked in TWO FILES, by two agents, at 23 and 16 aligned
differences. Each file's header says the other member "is still an
`INCLUDE_ASM`, so there was no landed sibling to read": both agents found the
twin STUB and neither looked for a PARK on it. That is a whole agent-run of
duplicated measurement, and it is invisible unless someone asks.

So before you write a new file here:

    python3 tools/decomp/dngfam.py FUNC          # every member of the family
    ls docs/dng-parks/                           # is any member already parked?

If one is, **update that file instead of writing a second one.** A park that
moves from 23 differences to 16 with ten more rows in its table is the result;
a second file at 16 that does not know about the first is two results that
cannot be read together.

If the figures came from two different C SHAPES -- as those two did -- say so
in the file and keep both tables. The disagreement is evidence: those two
parks appear to contradict each other on whether the array-ref or the integer
address form matches, and the resolution (symbol base vs pointer-local base)
is a spelling neither agent tested. You only see that with both tables in
front of you.


## WHEN A PARK LANDS, DELETE ITS FILE

After batch 12 two of the seven files here named functions that were already
landed, so an agent told to `ls docs/dng-parks/` would have taken one for an
available residue. This directory means "still a stub and still measured",
and a file that is neither makes it lie.

Delete the file in the same change that lands the function. Nothing is lost:
the mechanism and the load-bearing spellings belong in the landed function's
comment in `csrc/`, anything that transfers to another function belongs in
`docs/matching-levers.md` -- which is indexed and which the brief tells every
agent to read -- and git has the file itself at its full content if anyone
ever wants the whole rejected table back. A third copy in a directory nobody
opens is not an archive.
