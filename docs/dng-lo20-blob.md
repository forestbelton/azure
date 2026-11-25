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

## lo20's SYMBOL BOUNDARIES ARE ALSO WRONG, and the metric counts two phantoms

Separate from the blob, and found by counting `jr $ra` in each listing (see
the lever of that name). In this unit:

* **7 stubs have no `jr $ra` at all** -- `func_lo20_800250B4`, `80025E3C`,
  `8002640C`, `80027008`, `80027328`, `80027534`, `800280FC` -- so each is a
  fragment HEAD whose body runs into the next symbol;
* **4 small stubs have two `jr $ra`** -- `80025514`, `8002553C`, `800255AC`,
  `800264D4` -- so each is TWO functions under one symbol, with the second
  function's symbol missing;
* **13 of 46 names handed to one agent were not functions at all.**

Run across all 116 carved objects, **every one of these cases is in lo20.**
The other 115 are clean, which is what makes this a bounded repair.

**TWO ALREADY-LANDED BODIES ARE PHANTOMS AND THE FUNCTION COUNT INCLUDES
THEM.** `func_lo20_80025FF4` and `func_lo20_80028208` are both literally

    void func_lo20_80025FF4(void)
    {
    }

and their predecessors `func_lo20_80025E3C` and `func_lo20_800280FC` have
**zero** `jr $ra`. They are those functions' epilogues, and they read 2 words
at 0 differing only because `void f(void) {}` compiles to `jr $ra` + `nop`.

**They are not wrong -- they are byte-correct, and the containers are
byte-identical with them in place.** They are simply not functions, so the
landed count is two higher than the work done, and when the boundaries are
repaired these two symbols disappear into their predecessors. Do NOT unsplice
them on their own: that gains nothing and loses a byte-correct definition.
Fix them with the boundary repair, in one step.

**Two joined bodies are already proven.** Measured against their two listings
CONCATENATED (which `ovlverify` cannot do, since it takes its target from the
per-function listing and stops at the spurious symbol):

    func_lo20_80026EC0 + 80026ED0   4+13 = 17 words   0 differing
    func_lo20_80027328 + 80027368  16+ 7 = 23 words   0 differing
    func_lo20_80026F04 + 80026F1C   6+14 = 20 words  11 differing

Neither interior symbol is a branch target of anything, so deleting it is
safe. Those bodies are in `docs/dng-parks/func_lo20_80026EC0.c`.

## ATTEMPT 1 FAILED: a `symbol_addrs` file does NOT reclassify the range

Tried 2026-11-25 in the one window with no agents running. **It did not work,
and the build had to be reverted.** Recording it so the next attempt starts
further along.

**What I did.** `splat/dungeon.bin.yaml` already declares
`symbol_addrs_path: splat/dungeon.bin_symbol_addrs.txt` and **that file did not
exist**, so I wrote it with 32 `type:func` entries -- the function starts
derived mechanically as the word after every `jr $ra` and its delay slot, which
gives 33 boundaries of which 24 open with `addiu $sp, $sp, -N` and the last is
the 547-word trailing data run.

**What happened.** `make splat` ran clean and emitted **nothing** for
`0x8001E814..0x80025040`. lo20's listings now begin at `0x800250B4`, the blob's
own listing was deleted and not regenerated, and `csrc/ovl_dng_lo20.c` still
carries `INCLUDE_ASM(..., func_lo20_8001E814)` -- so the build broke on a
missing listing.

**Why, as far as I can tell.** A `symbol_addrs` entry names a symbol and its
type; it does not override spimdisasm's own decision that the range is DATA.
That decision is what emits `dlabel func_lo20_8001E814` in the first place, and
it is made before any symbol list is consulted.

**So the next thing to try is the YAML, not the symbol list** -- which is what
`extra_syms`' own note says ("the real fix is a rodata subsegment for each of
the two objects"). For lo20 that note is only half right, because this blob is
CODE, so the shape wanted is probably:

    - [0x1829800, data]
    - [0x1829814, c, ovl_dng_lo20]        <- the 32 functions
    - [0x182F7B4, data]                   <- the 547-word tail
    - [0x1830040, c, ovl_dng_lo20]        <- the existing functions
    - [0x18337ec, data]

**with the open question being whether splat accepts TWO `c` subsegments of the
same name in one segment**, since that would have to produce one `.c` file.
Check that before spending another splat cycle.

## What the failure DID establish

* **The restore procedure works.** Both recovered listings were copied back
  from `docs/recovered-listings/` and `make check` plus `make containers`
  returned five byte-identical containers with zero undefined references. That
  path is now tested rather than assumed.
* **The saved b31f listing is the authoritative one, not my regeneration.** I
  had hand-built a 53-label copy; the saved copy has **52** and links clean.
  My 53rd was a `dlabel D_b31f_80174674` at offset 0, which is unnecessary --
  that symbol is satisfied by the absolute in `dungeon.bin_extra_syms.txt`.
* **The blob is definitively CODE, and the README's "neither is a function" is
  about the SYMBOL TYPE, not the contents.** `objdump -b binary -m mips:3000`
  on the range gives a clean prologue, register saves and a table lookup. Both
  statements are true once separated: the symbol `func_lo20_8001E814` is
  data-typed, and the range it covers holds ~32 real functions.
* **There are ZERO `jal` targets inside the range**, so every one of those 32
  functions is entered from outside it. No internal call could ever have
  established the boundaries, which is why the carve had nothing to work with.

## A WARNING ABOUT THE PROGRESS FIGURE

`make splat` moved the denominator from **545,689 back to 543,606 words** --
the b31f island's 2,083 words stop being counted as a function once the
listings are regenerated. The percentage therefore went 79.0% to 79.3% **with
no work done at all.** Ask `progress.py` and say what changed; never compare a
remembered percentage across a splat.

## ATTEMPT 2 FAILED TOO, and it was about the SPURIOUS SYMBOLS, not the blob

Separate from the blob: the eight joined fragment pairs. These are **measured
and correct** -- `tools/decomp/pairverify.py` puts all eight at 0 differing,
325 words in total:

    80026EC0+80026ED0   17w      80028050+800280F4   43w
    80027328+80027368   23w      80026A64+80026A84   56w
    800250B4+80025170   57w      80027008+80027070   66w
    80026C20+80026C88   43w      80026F04+80026F1C   20w

**And nothing references any of the eight tail symbols** -- each appears
exactly once in `csrc/` (its own `INCLUDE_ASM`) and once in `asm/` (its own
listing's `glabel`), and nowhere else in the tree. So the obvious move is to
splice each joined body over the head's stub, delete the tail's stub, and let
the one definition cover both symbols' words **without touching splat at
all.**

**That compiles and links, and the container FAILS by 8.40%:**

    FAIL  0x23a95e of 0x1a867f0 bytes differ (8.40%); first at 0x18302e0

`0x18302e0` is lo20 + 0x6AE0, i.e. vram ~`0x800252E0` -- *after* the earliest
joined pair -- and the difference runs to the end of the image. **So the
layout shifted**: removing a tail `INCLUDE_ASM` removes its words from the
`.text` stream at that file position, and emitting head+tail together at the
HEAD's position does not put them back where the image wants them.

**The cause is not yet established and should be before attempt 3.** The
candidate explanations, in the order worth testing:

* a stub that sits BETWEEN a head and its tail in file order would be
  displaced -- check the file order of all 52 `INCLUDE_ASM` lines against
  address order, because splat emits in address order but a previous
  integration may not have;
* `gcc` emits a function as one unit at its position in the source, so the
  two halves become contiguous at the head's slot while the tail's slot
  collapses -- which is only layout-neutral if the two were already adjacent
  with nothing between them;
* the removed stub's `.include` may contribute alignment or padding that the
  C definition does not.

**Reverted; the build is green at five byte-identical containers.** The eight
bodies are not lost -- they are in
`/home/case/scratch/azure-dng/agent-h171/parks/ovl_dng_lo20.c` and in the park
files, measured, and `pairverify` is in the tree to re-check them.

**What both attempts together establish: the spurious-symbol problem cannot be
fixed from the C side.** Attempt 1 showed `symbol_addrs` does not reclassify a
range; attempt 2 shows deleting a stub does not preserve layout. The repair
has to change the YAML so splat stops emitting the spurious symbols in the
first place, and the open question from attempt 1 stands -- whether splat
accepts two `c` subsegments of the same name in one segment.

## THE GATING QUESTION IS ANSWERED FROM SPLAT'S SOURCE: TWO `c` SUBSEGMENTS OF ONE NAME SHOULD WORK

Both failed attempts ended on the same unknown -- whether splat accepts two
`c` subsegments with the same name in one segment, since both would target one
`.c` file. **Answered by reading the installed splat rather than by burning a
`make splat` cycle.** This is code reading, NOT a test, and should be treated
as such:

* `segtypes/common/c.py:129` -- `out_path` is
  `src_path / dir / f"{name}.{file_extension}"`, so two same-named `c`
  subsegments do both resolve to `csrc/ovl_dng_lo20.c`;
* `segtypes/common/c.py:225` -- `if not c_path.exists() and
  options.opts.create_c_files:`. **Splat only CREATES a `.c` file; it never
  overwrites one that exists.** That is why every hand edit to `csrc/` has
  survived every `make splat` in this project, and it means the second
  subsegment cannot clobber the first's output -- neither writes anything;
* `segtypes/common/c.py:348` -- a per-function listing is skipped when it
  already exists, which is moot because `make splat` begins `rm -rf asm`;
* the only "already paired" error (`scripts/split.py:151`) is about pairing a
  text segment with its **rodata** sibling. lo20 declares no `.rodata`
  subsegment, only `data`, so it cannot trigger.

**So attempt 3 is the four-subsegment layout, and the shape to try is:**

    - [0x1829800, data]
    - [0x1829814, c, ovl_dng_lo20]     the ~32 functions in the blob
    - [0x182F7B4, data]                the 547-word trailing data run
    - [0x1830040, c, ovl_dng_lo20]     the existing functions
    - [0x18337ec, data]

**What to verify the moment it runs, before anything else:** that
`asm/nonmatchings/ovl_dng_lo20/` gains listings in the `0x8001E814`..`0x80025040`
range. Attempt 1 produced *no* listing for that range at all, which is the
symptom to watch for. And restore both recovered listings from
`docs/recovered-listings/` afterwards -- that path is tested and works.

**Do it only with no agents running**, because `make splat` begins
`rm -rf asm build/asm` and every agent measures against those listings.

## ATTEMPT 3 FAILED, AND IT FOUND THE REAL BLOCKER: THE LINKER SCRIPT

Tried the four-subsegment layout with no agents running. **It does not work,
and the reason is NOT the one my source reading predicted.**

I had read `segtypes/common/c.py:225` and concluded two `c` subsegments of one
name are safe because splat only CREATES a `.c` file and never overwrites one,
so neither writes anything. **That part is true and it is not the problem.**
`build/dungeon.bin.ld` comes out as:

    build/asm/data/1829800.data.o(.data);
    ovl_dng_lo20_TEXT_START = .;
    build/csrc/ovl_dng_lo20.o(.text);
    build/asm/data/182F7B4.data.o(.data);
    build/csrc/ovl_dng_lo20.o(.text);      <- THE SAME OBJECT, TWICE

**One object's `.text` named twice cannot be split around the data.** `.text`
is a single section; the second reference places nothing. splat emitted
listings for **zero** functions in `0x8001E814..0x80025040` -- the range stayed
data -- and the layout could never have worked even if it had.

**I also tested and ruled out the `extra_syms` absolute.** Commenting out
`D_lo20_8001E814 = 0x8001E814;` (a DATA symbol at the blob's start, the
obvious suspect) changed nothing: still zero listings in the range.

Reverted: yaml and extra_syms restored from backups, `make splat` re-run, both
recovered listings copied back, **five containers byte-identical.**

## SO ATTEMPT 4 NEEDS TWO DIFFERENTLY-NAMED `c` SUBSEGMENTS

    - [0x1829800, data]
    - [0x1829814, c, ovl_dng_lo20a]     <- its own .c file
    - [0x182f7b4, data]
    - [0x1830040, c, ovl_dng_lo20]      <- the existing file, unchanged
    - [0x18337ec, data]

That gives two objects and two `.text` sections, which the link CAN place
around the data. The cost is real and should be weighed before starting:

* a new `csrc/ovl_dng_lo20a.c` with its own declaration block;
* the Makefile and any per-unit tooling must learn the new unit name --
  `dngfam`, `dngpick`, `ovlverify`'s unit lookup and `jtcheck` all key off
  `ovl_dng_*`, so check each;
* `symbol_name_format: lo20_$VRAM` must stay identical for both, or every
  symbol in the blob is renamed;
* the 547-word data run needs its own `data` subsegment name too.

**What all three attempts establish together:** the blob cannot be reached by
a symbol list (attempt 1), by deleting a stub (attempt 2), or by two
same-named `c` subsegments (attempt 3). The only route left is a second UNIT,
and that is a carve change rather than a decompilation one.

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
