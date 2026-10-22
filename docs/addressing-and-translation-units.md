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
   left at its default `-G0`, which is what maspsx recommends. **This is now
   proved on a real function rather than inferred -- see the next section.**

## Proved: one unit CAN mix both modes, and the scope is small

Measured 2026-10-22. This supersedes the statement that 27 functions "need both
modes and cannot match under either" -- they need both, and both are available.

**The mechanism.** A tentative definition in C (`s32 *D_800814E0;`, no
`extern`) makes cc1psx emit `.comm D_800814E0,4`; maspsx's
`--use-comm-section` turns a small `.comm` into `.sbss`; and gas addresses a
symbol it can see in `.sbss` `$gp`-relatively **even at the forced `-G0`**,
while every *undefined* symbol in the same unit still gets the absolute
`lui %hi` / `%lo` pair. So the mode is chosen per symbol, by whether the unit
defines it, instead of per compilation.

**The proof.** `func_800410FC` is five instructions and is exactly the
supposedly impossible case -- one `%gp_rel` store and one absolute
`%hi`/`%lo` pair:

    s32 *D_800814E0;                      /* definition, not extern */

    void func_800410FC(void) {
        D_800814E0 = (s32 *) D_80083C68;
        D_80083C68[0] = NULL;
    }

`verify.py` reports MATCH, 5 of 5 words, and the relocations confirm it is not
a coincidence of the masking:

    lui   v1,0x0      R_MIPS_HI16    D_80083C68
    addiu v0,v1,0     R_MIPS_LO16    D_80083C68
    sw    v0,0(gp)    R_MIPS_GPREL16 D_800814E0
    jr    ra
    sw    zero,0(v1)  R_MIPS_LO16    D_80083C68

**What it costs.** A defined object is allocated by the linker, and the
GPREL16 addend is computed from its final address, so each one has to be
*placed* at its original address: linker-script placement plus carving those
bytes out of the final `[0x54240, bin]` blob so they are not emitted twice.
That is the whole remaining cost, and it is bounded -- see below. Note the
encoding depends only on the final `$gp` offset, not on the symbol name, so the
C side does not need the original names: one struct covering a window, with the
fields at the right offsets, produces identical instructions.

**Define them in `.sdata`, not as C tentative definitions.** A tentative
definition becomes `.comm` and lands in `.sbss`, which is NOBITS: it occupies
vram without occupying ROM, so splitting the bin blob around it would shorten
the image by the window's size and shift everything after it. These windows are
genuinely *in* the file -- 224 zero bytes at 0x54C50 -- so the section has to
be PROGBITS. A top-level `__asm__` block does it, keeps the symbol small and
defined, and needs nothing from the compiler:

    __asm__(".sdata\n\t.globl D_800814E0\n\t.type D_800814E0,@object\n"
            "\t.size D_800814E0,4\nD_800814E0:\n\t.word 0\n\t.text");

Measured on func_800410FC: MATCH, 5 of 5, a 4-byte PROGBITS `.sdata`, and the
same GPREL16-beside-HI16/LO16 relocations as above. Gaps between the window's
objects are `.space`, and the whole window can go in one block in address
order.

**The placement step, concretely.** splat 0.50.1 has an `sdata` subsegment
type (`splat/segtypes/common/sdata.py`), so the closing blob gets split around
the window and the window named after the C file that defines it:

      - [0x54240, bin]
      - [0x54c50, sdata, <the C file>]     # vram 0x80081450, 224 bytes
      - [0x54d30, bin]

so splat emits `build/csrc/<file>.o(.sdata)` at that address. Note the
generated script ends with `/DISCARD/ : { *(*) }`, so a section splat does not
place is silently dropped -- which is why this needs the subsegment and not
just a definition. Unresolved: whether that subsegment also makes splat try to
*extract* the window into `asm/data/`, and if so how to suppress it. Settle
that first; it is the only unknown left in the chain.

### The scope, measured

The 31 `$gp`-blocked functions reference **38 unique symbols**, and they sit in
two narrow windows:

| window | vram | file offset | symbols | bytes | nonzero |
|--------|------|-------------|---------|-------|---------|
| A | 0x80080AA4..0x80080B0A | 0x542A4 | 18 | 108 | 25 |
| B | 0x80081450..0x80081526 | 0x54C50 | 20 | 224 | **0** |

**Window B is entirely zeros**, so those twenty objects need no initial values
recovered at all -- a zero-filled window and `.sbss` agree by construction.
And window B is where the demand is:

* **20 of the 31 functions touch only window B**, most of them exactly one
  symbol: func_8003F240, func_8003F2A4, func_8003F368, func_8003F5E0,
  func_8003F624, func_800410FC, func_80041134, func_80041AE4, func_80041B98,
  func_80041BE4, func_80041C64, func_80043D04, func_80043DB8, func_80043E04,
  func_80043E60, func_800448BC, func_80045340, func_800453E0, func_80047338,
  func_80047468.
* 9 touch only window A: func_8003CB08, func_8003CCB0, func_8003D92C,
  func_8003E4FC, func_8003F5AC, func_8004437C, func_8004450C, func_80044698,
  func_8004713C.
* 2 touch both: func_8003E34C, func_8003E758.

Only 4 of the 31 have no absolute reference at all, so the mixing really is
needed by 27 of them -- which is why this mechanism, rather than a per-file
`-G`, is the way in.

**So the decision is smaller than it was framed as.** It is not "move the
project's data into C". It is: carve one 224-byte all-zero window out of the
closing bin blob, place it from the linker script, and define its twenty
objects in C. That unblocks 20 functions. Window A is a second, slightly harder
step, because 25 of its 108 bytes are initialised and those values have to be
carried across.

### Window B's layout, derived and assembled

The widths below are what the accesses prove, counted over every listing in
`asm/nonmatchings`:

| offset | symbol | width | accesses seen |
|--------|--------|-------|---------------|
| 0x1450 | D_80081450 | 4 | lw, sw, and lbu of its first byte |
| 0x14B0 | D_800814B0 | 4 | sw |
| 0x14B4 | D_800814B4 | 4 | sw |
| 0x14C8 | D_800814C8 | 4 | sw |
| 0x14CC | D_800814CC | 4 | lw, sw |
| 0x14D0 | D_800814D0 | 1 | lbu, sb (29 accesses -- the busiest) |
| 0x14D4 | D_800814D4 | 1 | sb |
| 0x14E0 | D_800814E0 | 4 | lw, sw |
| 0x14E8 | D_800814E8 | 2 | lh, sh |
| 0x1500 | D_80081500 | 2 | lh, sh |
| 0x1508 | D_80081508 | 4 | lw |
| 0x1510 | D_80081510 | 4 | lw, sw |
| 0x1518, 0x151A, 0x151C, 0x151E | | 2 each | lhu, sh (and one lbu each on the first two) |
| 0x1520, 0x1522, 0x1524, 0x1526 | | 2 each | lhu, sh (and one lbu each on the first two) |

A generated `.sdata` block for exactly this layout, with `.space` for the gaps,
is staged at `gpdata_81450.c` in this session's scratchpad. It **assembles
correctly already**: 0xE0 = 224 bytes of `.sdata`, twenty symbols, each at the
right relative offset (D_800814B0 at +0x60, D_800814C8 at +0x78, D_80081526 at
+0xD6). And the `$gp` arithmetic checks out against the target listings --
`gp = 0x80080994`, so D_80081510 is at offset 0xB7C, which is what
func_80045340's `sw $zero, %gp_rel(D_80081510)($gp)` encodes.

Still unverified, and the next thing to do: the placement half. The instruction
forms are proved; nothing yet confirms that a carved window plus linker
placement reproduces the image byte-for-byte. Do that on one function --
func_800410FC is the smallest -- before touching the rest.

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
* `.extern SYM, 16` does not stop gas choosing GPREL16 at `-G8`; and in the
  other direction, a top-level `__asm__(".extern D_800814E0,4");` does **not**
  make gas choose GPREL16 at `-G0` either. gas's small-data decision for an
  undefined symbol follows `-G` alone, which is why the fix is to define the
  object rather than to describe it.
* Declaring the symbol as an array, or as a struct padded past the `-G`
  limit, makes gcc copy the address into a register and use displacements.
  That was recorded here as the opposite problem; it is in fact the lever for
  the register-base shape -- see "Which of the two shapes a declaration
  produces" above.
