# Global addressing, and what it reveals about the original files

## The observation

A global access in this game compiles two different ways, and which one is
correct is a property of the *function*, not of the symbol:

| function      | target form                  | matches when GNU as gets |
|---------------|------------------------------|--------------------------|
| func_8003F5D4 | `lw $v0, %gp_rel(D_800814C8)($gp)` | `-G8` |
| func_80041110 | `%gp_rel(D_800814E0)($gp)`         | `-G8` |
| func_8004066C | `lui %hi` + `lw %lo(D_80080A7C)`   | `-G0` |
| func_80040044 | `lui %hi` + `lw %lo(D_80081490)`   | `-G0` |

Size is not the discriminator. All four symbols are small-data objects that
physically live in `.sdata` or `.sbss`:

    .sdata  80080994-80081437      D_80080A7C, D_800814C8 (via .sbss)
    .sbss   80081438-800815bf      D_80081490, D_800814E0

## Why

`ccpsx` does not choose. It emits a symbolic access and leaves the decision to
the assembler:

    lw  $2,D_80080A7C
    addu $2,$2,$4
    sw  $2,D_80080A7C

ASPSX 2.56 expands that to `lui %hi` + `lw %lo` -- verified by assembling this
exact output with the real assembler via `ccpsx -c`, which emits the lui/%lo
pair and the 0x52/0x54 relocation pair. GNU as, given a nonzero `-G`, instead
assumes an *undefined* symbol is small data and emits `GPREL16`.

So the original build produced `$gp`-relative accesses only where ASPSX could
*prove* the symbol was small, which means only where the symbol was **defined
in the same translation unit**. An imported symbol got absolute addressing
even though it ends up in `.sdata`.

## What follows

**The addressing mode is evidence of original file membership.** A function
that reaches a global `$gp`-relatively was compiled in the translation unit
that defined that global. A function that reaches it absolutely was not.

That has three consequences.

1. No single `-G` setting can match everything, because the current C files
   each contain functions from several original translation units. The flag is
   per-compilation, so the fix is per-file -- see the Makefile hook below.

2. The `$gp`-relative accesses cluster functions into candidate translation
   units, which is independent evidence for where to split `csrc/unk_*.c`.
   Functions sharing a `$gp`-relative global very likely shared a `.c` file.

3. Eventually each C file should *define* the globals its functions reach
   `$gp`-relatively, rather than declaring them `extern`. At that point
   maspsx can see the size and will emit `%gp_rel` itself, and gas can be
   left at its default `-G0`, which is what maspsx recommends.

## Per-file override

`MASPSXFLAGS` is a normal make variable, so a single file can opt out:

    # This unit's functions reach their globals absolutely, so leave gas at
    # -G0 and let maspsx expand lui/%lo.
    build/csrc/some_unit.o: MASPSXFLAGS := $(MASPSXFLAGS_G0)

The project default is `--dont-force-G0` (gas at `-G8`), which suits the
`$gp`-relative majority; 163 accesses are `$gp`-relative against 271 absolute,
but the absolute ones are concentrated in the register-held-base shape that is
not reproducible from C yet anyway.

## Dead ends, recorded so they are not retried

* maspsx is faithful. The real ASPSX produces the same output once gas is
  left at `-G0`; this was never a maspsx bug.
* `-O1` vs `-O2`: both emit the symbolic form. `-O0` uses a register base but
  adds a stack frame.
* `-mno-gas` is rejected by cc1psx. `-mno-gpopt`, `-mmemcpy`, `-fforce-addr`
  and `-fforce-mem` have no effect.
* `.extern SYM, 16` does not stop gas choosing GPREL16.
* Declaring the symbol as an array, or as a struct padded past the `-G`
  limit, makes gcc copy the address into a register and use displacements --
  the opposite problem.
