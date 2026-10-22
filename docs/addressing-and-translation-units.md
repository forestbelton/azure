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

    # This unit's functions reach their globals $gp-relatively, so gas has to
    # be left at -G8 rather than maspsx's forced -G0.
    build/csrc/some_unit.o: MASPSXFLAGS := $(MASPSXFLAGS_G8)

The project default forces gas to `-G0` and lets maspsx expand `lui`/`%lo`,
which suits the absolute majority: 163 accesses are `$gp`-relative against 271
absolute, and only `gprel_12DD4` and `gprel_14910` are carved out the other
way. Many of the absolute ones hold the `%hi` half in a register shared across
several accesses; that shape is the 4.1 compiler's split-address code
generation (see `docs/compiler-version.md`), reproducible from C by accessing
the global through its struct or array type.

## Which of the two shapes a declaration produces

This is a property of the *declaration*, not of the symbol, and it is the
lever to pull when the addressing comes out wrong. With `-G8`:

| declaration in `structs.h`      | gcc emits for an access            |
|---------------------------------|------------------------------------|
| scalar, or array of <= 8 bytes  | symbolic (`lbu $3,SYM`, `la $4,SYM`), the assembler expands it |
| array of > 8 bytes, or `[]` with no extent | `lui ... # high` + `%lo` displacement, or a `lo_sum` in a register plus displacements |

So a global that the target reaches through a register base with
displacements must be declared as an array whose size is past the `-G` limit --
an incomplete `extern u16 D_80083858[];` is enough, and is the honest
declaration when the extent is unknown. A global the target reaches with one
`la`, or with the `lbu $3,SYM` form that the assembler expands, must stay a
scalar or a small complete array. `func_80045310` and `func_8003F270` are each
one instruction longer under the wrong choice; `func_8003F270` also needs the
small form to get the assembler-expanded `la SYM+1`/`lbu SYM+1` pair.

## Globals outside the loaded image are constant addresses

Symbols like `D_80013614`, `D_800E2968` and `D_80010A93` sit outside
0x8002d000..0x8008875f, so splat resolves them through
`undefined_syms_auto.txt`. In the original source they were not symbols at all
but integer constants cast to pointers, and for an *indexed* access the
difference is visible:

    u8 *p = (u8 *) 0x80013614;      /* lui 0x8001; addu idx; lbu 0x3614(reg) */
    extern u8 D_80013614[];         /* lui %hi; addiu %lo; addu idx; lbu 0(reg) */

splat renders the first form's `lui 0x8001` / `lbu 0x3614` pair as
`%hi(D_80013614)` / `%lo(D_80013614)` because the halves happen to add up to a
known symbol address, which makes the two look identical in the listing. They
are not: the constant-address form is one instruction shorter, because the low
half rides along as the load's displacement. Write these as
`*((u8 *) 0x80013614 + i)`, not as an `extern` array.

Two consequences:

* An access with no index is indistinguishable -- `lui $at,%hi(SYM)` /
  `sb $v0,%lo(SYM)($at)` is byte-identical to `lui $at,0x800E` /
  `sb $v0,0x2968($at)` -- so either spelling matches, and `func_8004397C`
  uses the `extern u8 D_800E2968;` spelling.
* decomp-permuter cannot score a function that uses the constant-address
  form. Its `target.o` comes from splat's assembly and carries an
  `R_MIPS_LO16` relocation against the symbol, where the constant form has no
  relocation and a literal displacement. The objects differ however good the
  source is, so the permuter never reaches zero. `make check` on the final
  bytes is the only authority for these.

## Dead ends, recorded so they are not retried

* maspsx is faithful. The real ASPSX produces the same output once gas is
  left at `-G0`; this was never a maspsx bug.
* `-O1` vs `-O2`: both emit the symbolic form. `-O0` uses a register base but
  adds a stack frame.
* `-mno-gas` is rejected by cc1psx. `-mno-gpopt`, `-mmemcpy`, `-fforce-addr`
  and `-fforce-mem` have no effect.
* `.extern SYM, 16` does not stop gas choosing GPREL16.
* Declaring the symbol as an array, or as a struct padded past the `-G`
  limit, makes gcc copy the address into a register and use displacements.
  That was recorded here as the opposite problem; it is in fact the lever for
  the register-base shape -- see "Which of the two shapes a declaration
  produces" above.
