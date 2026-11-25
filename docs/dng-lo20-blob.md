# ovl_dng_lo20's 6,667-word blob: ~32 functions counted as one stub

**Status: diagnosed, NOT fixed. The fix needs `make splat`, which must not run
while any agent is measuring.** `ovl_dng_lo20` and `ovl_dng_b31f` were both
held by agents when this was written.

## What it is

`func_lo20_8001E814` is listed as a single 6,667-word stub spanning
`0x8001E814..0x8002503C`. It is not a function. Its whole listing is `.word`
directives -- one `glabel`, no `endlabel`, no `jal`, no `jr $ra`, no prologue
instruction -- because spimdisasm never disassembled the range.

What is actually inside it, counted from the raw words:

    26  `addiu $sp, $sp, -N' prologues
    32  `jr $ra' returns
        code runs to 0x800247AC
        548 words past that with no return -- trailing data

So the range holds **roughly 32 real functions and about 6,119 words of code**,
plus ~548 words of data at the end. Leaf functions have no prologue, which is
why the two counts differ; 32 is the better estimate.

## Why it was never offered to anyone

It is in no family, so `dngfam.families()` has no entry and
`dngpick.stub_rows()` never yields it. It is one of only 21 stubs in that
position and the only one of them that is not a park. At 6,667 words it is
**5% of all remaining work** and the largest single row on the board.

## It is a known carve debt, not a new discovery

`splat/dungeon.bin.yaml` says so in its own comments:

> TWO OBJECTS STILL HAVE A COMPLAINT, and `dungeon_carve.py --check` prints
> them rather than hiding them: b31f and lo20, the two largest, carry rodata
> ISLANDS between functions (Shift-JIS strings in lo20 at +0x5020, 131 words
> across eleven runs in b31f). spimdisasm emits `.word` for what it cannot
> decode, so the bytes round-trip and the container is byte-identical; the cost
> is that the function after each island loses its own symbol. Both are worth a
> rodata subsegment each when somebody reads them.

**But the tool's current complaint for lo20 is NOT the island**, and the
difference matters:

    b31f   text +0x0108..+0xbf00 : 1.6% of text does not decode
    lo20   text +0x0014..+0x9fec : 15 internal jal targets off a start
                                   (+0x6840, +0x6970, +0x77f4, +0x7cd4)

b31f's complaint is undecodable text -- the islands. lo20's is that 15 `jal`
targets do not land on a detected function start, which is what makes
spimdisasm give up on boundary detection and emit the range as one blob.
**Do not assume the island is the cause here; the named starts (+0x6840 =
0x80025040 and the three after it) are where functions ARE detected, i.e. just
PAST the blob.**

## What fixing it would change, including the denominator

The 32 functions currently count as **one** stub. `progress.py`'s denominator
of 6,825 therefore under-counts the project by about 31 functions, and a
successful re-split makes the percentage go DOWN while unlocking real work.
That is the same effect the original carve had, and the rule from it stands:
**ask `progress.py`, never a remembered figure.**

## The order to do it in

1. Wait until no agent holds `ovl_dng_lo20` or `ovl_dng_b31f`.
2. Find the data boundary. Code ends by `0x800247AC`; the 548 words after it
   want a `data` or `.rodata` subsegment. Check the `+0x5020` island
   (`0x80023820`) separately -- it sits INSIDE the code range.
3. Resolve the 15 off-start `jal` targets, which are the actual blocker.
4. Edit `splat/dungeon.bin.yaml` -- lo20's subsegment list is at line 910.
   That file is ours; `splat/slus_006.14.yaml` belongs to another session and
   must never be staged.
5. `make splat`, then `make check` and `make containers`. All four containers
   must stay byte-identical: the carve rule is that a `bin` segment reproduces
   its bytes exactly, and a re-split must not change the image.
6. `rm` the four containers first if a change removes an input, or `make`
   no-ops the relink.

## Do not confuse this with the dead head block

The dungeon head block at `0x7804..0xbdd4` calls handlers that exist on no
container and is deliberately not split. **This is different**: the blob's
code decodes cleanly as R3000, has 32 returns, and sits in a live object whose
other 107 functions are ordinary stubs that agents are landing now.
