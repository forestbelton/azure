# Recovered listings

Two generated listings under `asm/nonmatchings/` were missing, and the build
did not notice for five batches. These are the recovered copies, kept here
because `asm/` is gitignored and `make splat` begins with `rm -rf asm`.

| listing | unit | size | vram | rom |
|---|---|---|---|---|
| `func_b31f_80174674.s` | `ovl_dng_b31f` | 0x208c (2,083 words) | 0x80174674 | 0x134CE74 |
| `func_lo20_8001E814.s` | `ovl_dng_lo20` | 0x682c (6,667 words) | 0x8001E814 | 0x1829814 |

## What they actually are

**Neither SYMBOL is a function.** Both have symbol type `O`, not `F`, in the
objects that reference them:

```
00009d6c g     O .text	0000208c func_b31f_80174674
00000000 g     O .text	0000682c func_lo20_8001E814
```

`func_b31f_80174674` is a table of pointers into b31f's own code, and the unit
also declares the same address as `extern u8 D_b31f_80174674[]` and uses it as
a facing table. `func_lo20_8001E814` occupies the first 0x682c bytes of lo20's
code subsegment, ending exactly where its first real function
(`func_lo20_80025040`) begins.

**BUT THE TWO CASES DIFFER IN WHAT THEY CONTAIN, AND THIS FILE USED TO CALL
BOTH OF THEM DATA. THAT IS WRONG FOR lo20.** b31f's island really is a table
of pointers. **lo20's range is CODE**, measured three ways on 2026-11-25:

    objdump -b binary -m mips:3000   a clean prologue, register saves and a
                                     jump-table lookup at the first word
    branch-target coherence          341 branch-shaped words, 340 of whose
                                     targets land INSIDE the range
    opcode plausibility              6,625 of 6,667 words (99.4%) carry a
                                     defined opcode; only 14 (0.2%) look like
                                     a RAM pointer

**A pointer table does not have 340 self-consistent branch targets.** The
range holds roughly **32 functions** -- 33 boundaries taken as the word after
every `jr $ra` and its delay slot, 24 of them opening with
`addiu $sp, $sp, -N` and the last being a 547-word trailing data run -- and
**zero `jal` targets inside it**, so every one is entered from outside.

**The symbol type is how splat DECLARED the range, not evidence about its
contents.** Three places in this tree read the type as though it settled the
question: this file, `docs/dng-parks/func_lo20_80026EC0.c`, and an agent's
hand-back which told me my own "collapsed text" description was wrong. It was
not wrong. See `docs/dng-lo20-blob.md` for the repair plan and for attempt
1's failure.

That is why splat emits no listing for either: it does not detect a function
there, so `make splat` -- which starts `rm -rf asm build/asm` -- destroys the
file and never puts it back. 79 of the 116 units declare a trailing `data`
subsegment; **b31f does not**, and lo20's blob is at the START of its `c`
subsegment rather than the end.

## Why the build stayed green

A listing is not a make dependency of the object that includes it. Both
objects were built while the listings still existed, so `make` simply reused
them, and all four containers stayed byte-identical from batch 16 to batch 24.

**The repo could not be rebuilt from clean during that window.** Deleting
either object was enough to break it, and the first edit to either `.c` would
have done so -- which is how this surfaced: an agent verifying a clone member
in b31f hit `can't open .../func_b31f_80174674.s for reading`.

## The shape of the b31f blob was established by the LINKER

It is this unit's table block and it DEFINES 52 interior symbols. Recovering
it took four link attempts, and each failure named exactly what was missing:

| labels emitted | undefined references | source of the label list |
|---|---|---|
| 1 (the outer `glabel`) | 28 | -- |
| 14 | 4 | `D_b31f_*` in `csrc/ovl_dng_b31f.c` |
| **52** | **0** | the C file **and all 97 sibling listings** |

A scan of the C file alone finds 14 of the 52, because **51 of the interior
tables are referenced from the unit's other `INCLUDE_ASM` stubs, not from
compiled source.** So the linker's undefined references were the only
surviving description of the region's contents -- there is no other record of
where one table ends and the next begins.

## How they were recovered

- **lo20** from `build/csrc/ovl_dng_lo20.o`, and its bytes are IDENTICAL to
  `build/dungeon.bin` at rom 0x1829814.
- **b31f** from `build/dungeon.bin`, NOT from the object. 53 of its 2,083
  words are relocation slots that read `0x00000000` in the object and are
  filled by the linker, so the object is not a faithful source. Each is
  emitted as its literal relocated value and annotated with the symbol it
  points at; 39 of the 53 resolve to a named `func_b31f_*`.

Both were then verified by deleting the two objects and the four containers
and rebuilding: `make check` and `make containers` reproduce all four
byte-identical -- `town.bin b946a57a`, `main.bin 69f63236`,
`dungeon.bin 27f9fcc3`, `slus_006.14 e6bfbb95` -- with zero undefined
references.

**Scope of that claim:** the two affected units rebuild from their recovered
listings and the containers are bit-exact. A full `rm -rf build` rebuild of
the whole repo has NOT been run, so "the repo rebuilds from clean" is
verified for these two units only.

## The permanent fix is a judgement call, and it is NOT done

Restoring the files makes the tree reproducible again, but `make splat` will
delete them once more. The two real options:

1. **Declare the blobs in the splat yaml** (give b31f a trailing `data`
   subsegment, and split lo20's `c` subsegment so the blob is its own `data`
   one). Correct and permanent, and it makes splat regenerate them. But it
   moves a subsegment boundary, which is exactly what silently changed
   `TEXT_START` and broke `dungeon.bin` when the b38 `.rodata` boundary was
   moved -- `.rodata, UNIT` takes its size from the compiler's output, not
   from the yaml range.
2. **A make rule that copies these files back** into `asm/nonmatchings/`
   after a split. Safe and ugly; no layout risk.

Option 1 is right and needs a deliberate decision, because it touches the
layout of two units. Until then, `tools/decomp/lstcheck.py` detects the
regression in a second, and should run before every batch.
