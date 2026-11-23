# Decompiling DUNGEON.BIN's 116 carved objects

This block is the 2,566 stubs that `docs/dungeon-map.md` counted and
`splat/dungeon.bin.yaml` carved on 2026-11-22. It is NOT the dungeon overlay
at 0x800894A0 -- that one is 100% landed and `docs/dungeon-sweep.md` is its
record. These are the objects DUNGEON.BIN streams into two RAM slots: 47
banked room scripts at 0x80170800, 66 at 0x80024000, and three low-memory
units.

## THE ONE FACT THAT SHAPES THE WHOLE SWEEP

**1199 of the 2575 stubs are exact copies of a sibling -- 93,302 of 327,714
words. 1376 distinct functions cover the block.**

The objects statically link a shared library, so one function appears,
relinked, in up to 216 of them. The copies differ only in the symbols the
relink changed.

    python3 tools/decomp/dngfam.py              # families by word yield
    python3 tools/decomp/dngfam.py FUNC         # FUNC's family, listed
    python3 tools/decomp/dngfam.py --diff A B   # why two bodies differ

So **the unit of work is a FAMILY, not a function.** Hand an agent the
representative, and replicate the landed body mechanically:

    python3 tools/decomp/dngland.py FUNC --from <agent copy>.c

which splices the agent's body into the real `csrc/<unit>.c`, verifies it
THERE, and then runs `dngclone.py` over every other member -- renaming each
copy's symbols and verifying each one against its own listing before writing
it. Nothing is written that does not come back `0 differing`.

Rank work by `yield`, not by word count. A 25-word function with 94 copies is
worth more than a 300-word function with one.

## The dispatch loop

Five agents at a time, one function each, from `tools/decomp/dng-brief.txt`
with `@FUNC@`/`@UNIT@`/`@WORKDIR@`/`@WORDS@`/`@COPIES@`/`@YIELD@` filled in.
Working directories go under `/home/case/scratch/azure-dng/agent-<id>/`,
**not** `/tmp` -- a routine cleanup once took four finished functions and five
unit copies with it (`docs/dungeon-parks/README.md`).

Two rules the brief repeats because they are the ones that cost other people
work:

* **No agent runs `make` in any form.** It re-runs splat, which deletes
  `asm/nonmatchings/` and rewrites it -- the ground truth every other agent is
  measuring against. The build gate is the dispatcher's, run once per batch.
* **No agent runs the permuter.** Its jobs are a machine-wide budget of 20 and
  only one search may run at a time; with five agents live that budget belongs
  to whoever is dispatching.

The agent's deliverable is the body, its comment, its declarations, and either
`0 differing` or a park in `docs/dng-parks/` with a measured residue.

## `ovlverify.py` resolves these by NAME, not by address

47 of the objects load at 0x80170800 and 66 at 0x80024000, so an address names
a RAM SLOT here and not an object: no range table can ever say which unit a
function belongs to. splat gives each segment `symbol_name_format: <tag>_$VRAM`
instead, so a function is `func_<tag>_<vram>` -- `func_b01_801708E8` -- and the
name itself carries the answer. `overlay_for()` consults that first.

The consequence for anything else that reads a function name: the old
`func_[0-9A-Fa-f]{8}` pattern does not match these, and `int(name.split("_")[1],
16)` parses `b01` as 0xB01 rather than failing. Tools that key on either need
checking before they are trusted on this block.

## The build gate is still the build gate

`ovlverify` compares ONE function's words. It says nothing about a unit's
.rodata, and a local array initialiser emitted into .rodata that an
`INCLUDE_RODATA` line also pulls in has silently moved every later function
in a unit before. A clean sweep here is a reason to build, not a substitute:

    make check > /tmp/check.log 2>&1; echo rc=$?
    make containers

and **count the `OK  byte-identical` lines: there must be four**, two of them
from `make containers` rebuilding town and main. Read the hashes, never pipe
make through `head`/`tail`, and do not `rm -rf build` -- splat generates the
linker scripts into it.

## THE COMMITTED TREE DID NOT RELINK, AND NOTHING CAUGHT IT FOR A DAY

Fixed 2026-11-23 in `splat/dungeon.bin_extra_syms.txt`. Two addresses --
`0x80174674` in b31f and `0x8001E814` in lo20 -- are rodata islands that the
carve named as functions. spimdisasm emits `dlabel func_<tag>_<vram>` there
while the code reaching the address gets splat's data spelling
`D_<tag>_<vram>`; the two names never meet and the dungeon link dies on
`undefined reference to D_b31f_80174674'.

It is LATENT, which is the part worth remembering: the carve's own build was
byte-identical because the listings on disk had been written by an earlier
splat run, and the failure appears only when something re-runs splat. A
landing batch does, so the first batch after the carve inherited a broken
build and it looked like the batch had broken it. **If a dungeon link fails
right after a sweep, check whether it fails at HEAD too before reading a line
of the diff** -- `git stash push csrc/`, `rm -f build/dungeon.bin`, build.

A grep for `^dlabel func_` over the carved listing directories finds exactly
these two.

## Batch 1, 2026-11-23: five agents, 551 functions, 15,994 words

Five representatives, one agent each, all five landed -- four on the first
compile. Then `dngland.py` cloned each family:

| representative | words | copies | cloned | yield |
|---|---|---|---|---|
| `func_b01_801735D4` | 14 | 216 | 215/215 | 3,024 |
| `func_b01_80173568` | 8 | 119 | 118/118 | 952 |
| `func_b01_80172800` | 25 | 94 | 93/93 | 2,350 |
| `func_b01_80172628` | 81 | 46 | 45/45 | 3,726 |
| `func_b02_80174574` | 50 | 42 | 41/41 | 2,100 |
| `func_b03_80171DA0` | 113 | 34 | 33/33 | 3,842 |

The block went 4259/6825 to 4810/6825, 39.7% to 42.7%.

**A FAMILY CAN SPLIT ON ONE IMMEDIATE.** `func_b03_80171DA0`'s last five
members compare `0x3F`/`0x7E` where the other 29 compare `0x40`/`0x80`.
`dngfam.py` normalises hex literals, so they are one family there; `dngclone`
refused them at 2 differing and wrote nothing, and they landed from the same
body with the other constant. That is the normalisation working as designed:
the cost of a too-loose family is a failed verify, never a wrong landing.

Three defects the dry runs caught before anything reached `csrc/`, each now
written up at its site in `tools/decomp/dngclone.py`:

* the symbol pattern required the `<tag>`, and a per-copy data label is
  tagged inside the object's carved data subsegment but UNTAGGED in the `bin`
  tail -- one family had both spellings in the same instruction pair;
* the thread pool ran one thread per MEMBER, and a family with five members
  in one unit had them overwrite each other, every one reporting
  `0 differing` against a file that was dead by the time it was on disk;
* `\s*$` in the splice pattern ate the blank line after a stub, so two landed
  functions ended up adjacent, and the comment walk-back -- which looked back
  to the last blank line -- then swallowed the previous function whole and
  cloned it as a second definition.

And one that reached `csrc/` and had to be re-run: a clone carried its whole
unit's declaration block rather than the declarations its body uses, so each
family after the first copied the earlier families' declarations into every
member and renamed only the ones its own symbol map knew. 171 stray
declarations across 46 units, harmless to the link and wrong in every other
way. `needed_decls()` is the fix.
