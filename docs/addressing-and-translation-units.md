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
pair and the 0x52/0x54 relocation pair. GNU as at a nonzero `-G` can instead
emit `GPREL16`.

**Exactly when it does was recorded wrongly here, and the correct rule is a
lever.** gas does *not* assume an undefined symbol is small. It reads the size
from a `.extern SYM,size` record and goes `$gp`-relative only for
`0 < size <= G`. Measured directly against `mips64-linux-gnu-as -G8`:

| input | result |
|-------|--------|
| `.extern foo, 4` + `lw $2,foo` | `lw v0,0(gp)`, GPREL16 |
| `.extern foo, 16` + `lw $2,foo` | `lui` + `lw`, HI16/LO16 |
| `.extern foo, 0` + `lw $2,foo` | absolute |
| no `.extern` at all | **absolute** |

And cc1psx emits that `.extern` line with the declared type's size. So the
`$gp`-relativity at a nonzero `-G` is driven by the C declaration, through
cc1psx's own `.extern`, not by gas guessing about imports.

The awkward part is that cc1psx uses the same threshold for a second decision:
a symbol of size `<= G` gets the **bare-symbol** form (left for the assembler
to expand) *and* its `.extern`, while anything larger or incomplete gets the
**compiler-split** `lui %hi` + displacement and, for an incomplete array, no
`.extern` at all. There is therefore **no C declaration that yields
bare-symbol-but-absolute** -- which is the shape func_80041AE4 and
func_80043D04 need for D_80081480 and D_8008148C while wanting `$gp` for a
`u16` in the same function.

**The fix is a per-unit `-G` that splits the two by size.** Those two functions
reach a `u16` (`.extern ...,2`) `$gp`-relatively and `u32`s
(`.extern ...,4`) absolutely, so gas at `-G2` gives both: 2-byte objects
`$gp`-relative, 4-byte ones absolute in the bare-symbol form the target has.
`MASPSXFLAGS_G2` in the Makefile does this for `gprel_152E4` and `gprel_17504`
only -- their existing contents assemble identically at `-G2` and `-G8`, while
the other four carved units differ and must stay at `-G8`. Measured: at `-G8`
func_80041AE4 comes out 44 of 45 words wrong; at `-G2`, with plain scalar
declarations and nothing added, it matches.

An `__asm__(".extern SYM, 16")` inside the function body also works, by
overriding the size cc1psx emitted (gcc flushes its `.extern` block once before
the first function, so a top-level `__asm__` lands ahead of it and loses). It
was measured working, but `-G2` needs nothing non-C in the source and is what
the tree uses.

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

**The mechanism, and it is maspsx's rather than gas's.** cc1psx emits the same
bare-symbol load or store either way (`sw $4,D_800814C8`). **maspsx rewrites it
to `%gp_rel(...)($gp)` itself, before gas ever sees it**, and gas's `-G` never
enters into the decision. From `maspsx/__init__.py`: while scanning its input
maspsx collects `sdata_entries` and `sbss_entries` -- a `.size SYM,N` line
*inside a `.sdata` section* registers `SYM`, and `.comm`/`.lcomm` feed
`sbss_entries` -- and then, for a load or store whose operand is a bare symbol,

    if gp_allowed and (symbol in self.sbss_entries or symbol in self.sdata_entries):
        res.append(f"{op}\t{r_dest},{gp_rel}")
    else:
        res.append(line)

So the mode is chosen per symbol, by whether **maspsx saw a definition in this
unit**, instead of per compilation. Verified by running maspsx on the two
cc1psx outputs by hand: identical input instruction, `%gp_rel` out with the
definition present and unchanged without it, with `-G8` passed both times.

Three consequences the "gas decides" wording hid:

* **The `.size` line is the load-bearing one**, not `.globl` or `.type`. That
  is what puts the symbol in `sdata_entries`.
* **The object's size does not gate the choice.** The size-versus-`-G`
  comparison applies to `.comm`/`.lcomm`, not to symbols defined in an explicit
  `.sdata` section, so a window object larger than 8 bytes still gets
  `%gp_rel`.
* **`.extern SYM, N` cannot do it**, and now for a concrete reason: `.extern`
  is a *terminator* of maspsx's `in_sdata` scan, never a producer of an entry.

**A C tentative definition also works**, by the `.comm` route: cc1psx emits
`.comm D_800814E0,4` and maspsx's `--use-comm-section` puts a small `.comm`
into `.sbss`, which is the other half of the same predicate.

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
order -- **measured, not inferred**: a probe with 120 bytes of `.space` filler
ahead of three objects still matches, so an object's position inside the block
does not affect the rewrite, several `.size` directives in one block all
register, and `.space` between them is skipped harmlessly.

**But the C declarations must stay small scalars.** maspsx's rewrite only fires
on a load or store whose operand is a *bare symbol*. Declare a window object as
a struct or array bigger than `-G8` and reach a field through it, and cc1psx
emits the compiler-split `lui %hi` / `%lo` pair instead -- no bare-symbol
candidate, no rewrite, absolute addressing. So "one struct covering a window"
is advice for the **assembly definition and the linker script only**; on the C
side, one small scalar declaration per object, which is what `include/structs.h`
already has.

**A PARK FILE DOES NOT INHERIT ITS UNIT'S `.size` BLOCK, and that is a whole
word of mis-measurement.** The `.size` records a carved unit needs live in one
`__asm__(".sdata\n\t.size SYM,4\n\t.text")` block at the top of the unit's
`.c` file. A candidate compiled standalone by `verify.py` has only what is in
that file, so a reference to one of those symbols comes out as the two-word
absolute form where the unit gets a one-word `%gp_rel`. func_8004DE88 was
parked at "64 of 65 words, one word SHORT" for two sessions on this: the shape
that matched it is 65 words with the block and 66 without, so the park was
measuring one word too few in a function whose real defect was elsewhere
entirely. `verify.py` and `advise.py` now WARN when a source references a
symbol its unit sizes and it does not, and print the block to paste. An audit
of all the remaining parks on 2026-10-27 found no other instance.

**One symbol can take both modes inside one function, free.** func_8003F624
stores through `%gp_rel(D_800814D4)($gp)` and then passes `&D_800814D4` to
`CdControlB`, which comes out `lui %hi` / `addiu %lo` -- confirmed in the
relocations as GPREL16 at one site and HI16/LO16 at the other, for the same
symbol. It follows from the predicate: an address materialisation is `la`,
which is neither a load nor a store, so maspsx leaves it alone and gas's `la`
macro expands it absolutely. Nothing in the source chooses this, but the
placement work has to get both forms right for the same object.

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

The `$gp`-blocked functions reference **38 unique symbols**, and they sit in
two narrow windows. (There are **32** such functions, not 31: the earlier count
missed func_80044618, whose only symbolic references are
`%gp_rel(D_80080AFC + 0x2)` -- an addend form that a naive grep for
`%gp_rel(SYM)` skips. Grep `%gp_rel([^)]*)` instead.)

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
* 10 touch only window A: func_8003CB08, func_8003CCB0, func_8003D92C,
  func_8003E4FC, func_8003F5AC, func_8004437C, func_8004450C, func_80044618,
  func_80044698, func_8004713C.
* 2 touch both: func_8003E34C, func_8003E758.

Only **5** of the 32 have no absolute reference at all -- func_8003E4FC (132
instructions), func_8003F5E0 (3), func_80044618 (35), func_80044698 (35) and
func_800448BC (24) -- so the mixing really is needed by 27 of them, which is
why this mechanism rather than a per-file `-G` is the way in. Those five are
the exception and want the **existing** carve-out instead: put them in their
own unit with `MASPSXFLAGS_G8`, exactly as `gprel_12DD4` and `gprel_14910`
already are, with no window definition and no linker placement. That is the
cheapest landing available in this whole group, and the five are the same five
this document named before my own first pass miscounted them.

**So the decision is smaller than it was framed as.** It is not "move the
project's data into C". It is: carve one 224-byte all-zero window out of the
closing bin blob, place it from the linker script, and define its twenty
objects in C. That unblocks 20 functions. Window A is a second, slightly harder
step, because 25 of its 108 bytes are initialised and those values have to be
carried across.

### Which boundaries are wrong, and the two functions that landed because of it

A `$gp`-relative reference means the function was compiled in the unit that
defined the global, so asking **which symbols are referenced from more than one
of the project's current units** finds boundaries that cut an original one.
Only **two of the 38 do**, which says the current boundaries are nearly right
and names exactly where they are not:

| symbol | referenced from | what it means |
|--------|-----------------|---------------|
| D_800814D4 | func_8003E758 (unk_11940), func_8003F624 (unk_12DEC) | the boundary at 0x8003F5E0 cuts an original unit |
| D_800814E0 | func_800410FC (unk_12DEC), func_80041134 (unk_14934) | so does the one at 0x80041134 |

The second was actionable immediately. func_80041110 already sat between those
two functions in a carved `-G8` unit of its own, so all three were one original
translation unit; moving that carve-out's two ends to take them in landed both,
with **no window definition and no data relocation**. `make check` is
byte-identical and the stub count went 42 to 40. The unit is now
`csrc/gprel_148FC.c`, renamed for its new start.

`verify.py <file> <unit> <func> --g8` models a carved unit (gas left at `-G8`
rather than maspsx's forced `-G0`), so the same check is cheap for the rest.
**Try this before reaching for a window definition**: it needs no linker
placement, no carving of the bin blob, and no new mechanism.

The first row is the obvious next candidate, though it is harder -- the
boundary at 0x8003F5E0 is where `gprel_12DD4` already sits, so absorbing
func_8003F624 means extending that unit across func_8003F5E0 and whatever lies
between.

### The method that actually worked: carve the cluster, don't place a window

Proved four times over. **No window definition and no linker placement are
needed** -- the existing `-G8` carve-out mechanism is enough, once the units
are cut where the original ones were.

Inside `unk_14934` (and `unk_11940`) the `$gp` symbols cluster into runs of
**consecutive functions**, one run per symbol: D_80081500 is reached by exactly
func_80043D04/DB8/E04/E60 and nothing else; D_800814E8 by exactly
func_80041AE4/B98/BE4/C64. Since a `$gp`-relative reference means the function
was compiled in the unit that *defined* the global, each run was its own
translation unit. splat lumps several into one `c` subsegment, and that is the
*only* reason a unit appeared to need both addressing modes at once.

So the recipe is:

1. Pick a cluster. Check the run is consecutive and note which functions in it
   are already decompiled.
2. **Audit every symbol the run reaches ABSOLUTELY.** At `-G8` gas makes any
   small *undefined* symbol `$gp`-relative, so each must either sit outside
   gp +/- 0x8000 or be declared so cc1psx compiler-splits the address itself
   (an incomplete array, or a struct larger than `-G8`). A symbol inside the
   window declared as a scalar costs a word -- D_80082E6E did exactly that.
   Symbols reached only from functions that are still `INCLUDE_ASM` stubs need
   no attention yet: the listing carries its own `%hi`/`%lo` and `-G` never
   touches it.
3. `python3 tools/decomp/carve.py UNIT FIRST LAST NEWUNIT TAILUNIT`, then add
   the two splat subsegments and the `MASPSXFLAGS_G8` entry it prints.
4. `make splat && make check`, then `nmcheck.py` and `annotate.py --self-test`
   -- a carve moves listings between units and both tools catch the near-miss
   files and self-test cases that still name the old one.

Carved so far: `gprel_12A40`, `gprel_152E4`, `gprel_17504`, `gprel_17B7C`,
plus the two boundary moves into `gprel_12DD4` and `gprel_148FC`. The chain
through the old unk_14934 now reads unk_14934, gprel_152E4, unk_154BC,
gprel_17504, unk_176B8, gprel_17B7C, unk_1811C.

**Where the carve cannot help**, and this is the one case the window definition
is still needed for: a function that needs **both modes for the same symbol**.
func_8003F624 stores to D_800814D4 `$gp`-relatively *and* passes
`&D_800814D4` to CdControlB, which the target reaches with an absolute
`lui %hi` / `addiu %lo` pair. At `-G8` gas makes the address-of
`$gp`-relative too, giving 18 differing words. maspsx's rewrite fires only on a
load or store, so the `.sdata` definition route *does* give both -- which is
why that function's verified source uses it. It stays blocked on placement.

One more case worth knowing from the same work: a cluster can stop at a
function that touches the same object. func_8003F240 reaches D_800814D0
`$gp`-relatively while func_8003F270, immediately after it, reaches the same
object **absolutely** and already matches that way. They cannot have shared a
unit, so that carve is one function wide.

### The group the carve could not reach: one object, both modes -- SOLVED

Four of the `$gp` functions reach **the same object both `$gp`-relatively and
absolutely**, which no unit-level setting can express: a `-G` value decides per
symbol SIZE and an `.extern` override decides per SYMBOL, and both apply to
every access of that symbol alike.

| function | the object, both ways | state |
|----------|-----------------------|-------|
| func_8003F624 | `sb %gp_rel(D_800814D4)` and `&D_800814D4` passed to CdControlB | landed, `csrc/unk_12DEC.c` |
| func_8004713C | `sh %gp_rel(D_80080B08)` and `lui/addiu %hi/%lo(D_80080B08)` into `$s5` | landed, `csrc/unk_19434.c` |
| func_8003F2A4 | `sw %gp_rel(D_800814B0)` and `lui %hi(D_800814B0)` | in progress |
| func_8003F368 | `sw`/`lw %gp_rel(D_80081450)` and `lui %hi(D_80081451)`; likewise `D_800814D0` against `sb %lo(D_800814D3)` | in progress |

**maspsx can express it, and it needs no linker placement.** Its `%gp_rel`
rewrite fires on a **load or store** and leaves an address materialisation
alone, which is exactly the split these functions need. What blocked the route
for months was that it was written as a `.sdata` **definition**:

    /* DO NOT USE: makes the symbol a defined object that has to be placed */
    __asm__(".sdata\n\t.globl D_800814D4\n\t.type D_800814D4,@object\n"
            "\t.size D_800814D4,1\nD_800814D4:\n\t.byte 0\n\t.text");

That puts a real 1-byte object in `.sdata`, and splat's legacy linker-script
generation cannot place one object's `.sdata` at a distant fixed address.

**It does not have to be a definition.** maspsx's `_uses_gp` consults only its
`sdata_entries` table, and the `.size` record alone populates that. So:

    __asm__(".sdata\n\t.size D_800814D4,1\n\t.text");   /* file scope */

gives the identical code while defining nothing. Verified side by side on
func_8003F624 -- both routes a 25-word MATCH -- and the objects differ only in
the one way that matters:

| | `.sdata` size | the symbol |
|---|---|---|
| definition route | 1 byte | `OBJECT GLOBAL` in `.sdata`, needs placing |
| size-record route | **0 bytes** | **`NOTYPE GLOBAL UND`**, size 1 |

With the symbol still undefined, the linker script's own
`D_800814D4 = 0x800814D4;` from `undefined_syms_auto` resolves it and there is
nothing to place. There is no `.globl`, no `.type`, no label and no `.word`, so
nothing can collide with that assignment either.

**This also removes the need for a window definition entirely**, which is what
the rest of this document was building towards. A unit does not have to be
carved to a particular `-G` for one of these: func_8004713C's two symbols are
the only `$gp` references anywhere in `unk_19434`, and it landed in that unit
at the project default `-G0` with the directive alone.

Two things to keep in mind when reaching for it:

* Give the **real size** of each symbol (1 for a byte, 2 for a halfword, 4 for
  a word) and one `.size` per symbol -- the rewrite is per symbol, not per
  object.
* It is also the fix for a maspsx gap that costs one word per site in a carved
  `-G8`/`-G2` unit: maspsx will not emit the load-delay nop before a `%gp_rel`
  store of a symbol it knows only from an `.extern` record, because `_uses_gp`
  skips `.extern` outright. See section 11 of `docs/matching-levers.md`.

The principled fix is in `vendor/maspsx`: record `.extern SYM,size` and let
`_uses_gp` consider those symbols when gas is left at a nonzero `-G`, gated on
`--dont-force-G0`. If that lands, every one of these directives comes back out.

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

### Window A, for later

108 bytes at 0x80080AA4, with eighteen referenced symbols: halfwords at
+0xAA4, +0xAA6, +0xAA8, +0xAAC, +0xABC, +0xABE, +0xAD2, +0xAD4, +0xAFC,
+0xB00, +0xB08 and +0xB0A; bytes at +0xAAA, +0xAD0 and +0xAF3 (that one `lb`,
so signed); and words at +0xAB0, +0xAD8 and +0xADC.

25 of its 108 bytes are nonzero, and only ONE of them belongs to a referenced
symbol: D_80080AF3 holds 0xFF, consistent with its signed `lb`. The other
nonzero runs belong to objects in the same small-data region that these
functions do not touch:

    80080AB4  82 6a 82 8f 82 88 00 00          Shift-JIS "Koh"
    80080AC0  00 c0 02 01  5a 07 00 00         0x0102C000, 0x75A
    80080AC8  00 c0 02 01  51 3a 00 00         0x0102C000, 0x3A51
    80080AE0  00 00 00 04  5c 39 00 00         0x04000000, 0x395C
    80080AE8  00 00 00 04  64 39 00 00         0x04000000, 0x3964
    80080B0C  20 00 80 00                      0x00800020

"Koh" is the protagonist's default name, which is good evidence that this
region is the game-state/save area rather than scratch. Carving window A means
carrying those bytes across verbatim; they stay at the same addresses with the
same values, so references to them -- whether by constant or by a symbol from
splat's undefined list -- keep resolving. It is strictly more work than window
B and it is not needed by the twenty functions window B unblocks, so do B
first.

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
