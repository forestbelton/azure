# Undeclared callees: 4 symbols, all in ovl_dng_lo16 (was 19 across 13)

Found 2026-11-25, after `declcheck.py` was fixed to pass `-Wimplicit`. Every
one of these is in **landed, byte-identical code**: the containers match, so
nothing here is a wrong byte. They are a **source-faithfulness** gap, not a
build problem -- gcc 2.7.2 gives an undeclared callee an implicit
`int ()` type, and where the arguments happen to pass in the same registers
either way the emitted code is identical.

The risk is latent, not present: a later edit near one of these call sites can
change the emitted code with no warning, because the compiler has no prototype
to check the call against.

| unit | undeclared callee |
|---|---|
| `csrc/ovl_dng_b02.c` | `func_b02_80174910`, `func_b02_80174DA0` |
| `csrc/ovl_dng_b06.c` | `func_b06_80172550` |
| `csrc/ovl_dng_b14.c` | `func_b14_80173A08` |
| `csrc/ovl_dng_b21.c` | `func_800ABDBC`, `func_800AC480`, `func_800AC5C8`, `func_800AC7EC`, `func_800ACE34` |
| `csrc/ovl_dng_b25.c` | `func_8003DB94` |
| `csrc/ovl_dng_b31f.c` | `func_8003DB94`, `func_80099FDC` |
| `csrc/ovl_dng_b38.c` | `func_b38_80172258` |
| `csrc/ovl_dng_lo16.c` | `bzero`, `VSync`, `func_8004AB7C`, `func_8004E634` |
| `csrc/ovl_dng_s41.c` | `func_800B8FC8` |
| `csrc/ovl_dng_s51.c` | `func_800478B8` |
| `csrc/ovl_dng_s72.c` | `func_8003FD64` |

## THREE MORE CLOSED, 2026-11-25 -- ONLY `ovl_dng_lo16`'s FOUR REMAIN

    ovl_dng_b31f  void func_8003DB94(void *, void *, s32);
    ovl_dng_b31f  void func_80099FDC(UnkNode *n);
    ovl_dng_b38   s32  func_b38_80172258(u8 *a, void *b, void *c, s32 d);

Each one's **call-site arity was checked against its definition first**, per the
rule below, and all three agreed -- so a real prototype was right and the
empty-paren form was not needed. `func_b38_80172258` is defined in its own unit
and called above that definition, so it needed a forward declaration rather
than a new fact.

**One note on `func_8003DB94`: the tree already carries three spellings of it**
-- `(void *, void *, s32)` in 40 units, a named variant of the same in 3, and
`(u8 *, u32, s32)` in 2. I used the 40-unit form rather than invent a fourth.
All three compile identically for pointer-sized arguments, which is why the
divergence has gone unnoticed; it is worth unifying one day but it is not a
correctness question.

**Still open: `bzero`, `VSync`, `func_8004AB7C` and `func_8004E634`, all in
`ovl_dng_lo16`** -- left alone because an agent held that unit. `bzero` and
`VSync` have real PSY-Q declarations; the other two want a prototype read off
their listings.

## SEVEN MORE CLOSED, 2026-11-25 -- AND THE EMPTY-PAREN FORM IS SOMETIMES THE RIGHT ONE

`ovl_dng_s41` took `void func_800B8FC8(u8 *, s32 *, s32 *, s32, s32);` and
`ovl_dng_s72` took `u8 *func_8003FD64(s32, u8 *);`, both real prototypes copied
from units that already had them.

**`ovl_dng_b21`'s five needed the EMPTY-PAREN form, and a comment in the file
said so before I started.** Two of them are *defined* with four parameters
(`s32 func_800AC480(u8 *p, UnkVec *v, u8 *r, u8 *b)`) while b21 calls them with
**one** (`if (func_800AC480(p) != 0)`), and the other three are **old-style
K&R definitions with untyped parameters**, for which `s32 f();` is the correct
C89 declaration rather than a cop-out.

I added real prototypes first and **broke the build** -- "too few arguments to
function `func_800AC480'". The source comment two lines above the call site
reads: *"The empty-paren `s32 func_800AC480();' already in this unit's
declaration block is what the one-argument call needs; do not rewrite it to a
prototype."* Its **instruction was right and its premise was wrong** -- those
declarations were never actually in the block, only quoted in the comment,
which is why `declcheck` flagged them. Adding the empty-paren form closes all
five: `declcheck b21` is now 0 warnings and the containers stay
byte-identical.

**So the rule for this backlog is not "add a prototype".** It is: read the
call sites' arity first, and where it disagrees with the definition, the
empty-paren declaration is the faithful spelling -- the original source cannot
have had a prototype there either, or it would not have compiled.

## FIVE CLOSED, 2026-11-25, gated at five byte-identical containers

The in-unit cases were unambiguous -- the callee is **defined in the same
file**, so its own definition is the prototype -- and one was a copy of a
declaration another unit already had:

    csrc/ovl_dng_b02.c  void func_b02_80174910(u8 *, u8 *);
    csrc/ovl_dng_b02.c  void func_b02_80174DA0(u8 *, void *, u8 *);
    csrc/ovl_dng_b06.c  s32  func_b06_80172550(u8 *, void *, void *, s32);
    csrc/ovl_dng_b14.c  s32  func_b14_80173A08(u8 *, void *, void *, s32);
    csrc/ovl_dng_s51.c  void func_800478B8(u8 *);

All five were codegen no-ops, as expected: an `s32` return is `int`, a `void`
return at a call whose result is unused is the same, and pointer arguments do
not promote. **That was the prediction, and `make containers` was the check --
a prototype whose argument width differs from the implicit one CAN change the
emitted code, which is the whole reason these are worth closing one unit at a
time.**

Not touched, and why: `ovl_dng_b25`, `ovl_dng_b31f` and `ovl_dng_lo16` are held
by live agents, and the five `func_800Axxxx` callees in `ovl_dng_b21` plus
`func_800B8FC8` and `func_8003FD64` need a prototype read off their own
listings rather than copied.

`bzero` and `VSync` are library functions with real PSY-Q declarations;
`func_8003DB94` and `func_800478B8` are declared in several other units and
just need copying. The rest want a prototype read off their own listings.

**Fixing one is gated like any other change:** add the declaration, re-run
`ovlverify` on every body in the unit, then `make check` and `make containers`
for five byte-identical lines. A prototype that changes an argument's width can
change the emitted code, which is exactly why these are worth closing.

## `rand` is NOT in this list, and must not be "fixed" the same way

71 units call `rand` undeclared, and a blanket `#include "rand.h"` in
`include/common.h` **breaks the build** -- it was tried and reverted on
2026-11-25. `psyq/include/rand.h` declares `extern int rand(void);`, but this
project's call sites are heterogeneous on purpose:

* `csrc/ovl_dungeon_800C2824.c:1525` calls `rand(ii)` -- *with an argument*,
  which the real prototype rejects (`too many arguments to function 'rand'`);
* `csrc/ovl_town.c`'s own comments record `rand(0x28)` and `rand(0x40)` as
  **measured** facts, naming the `$a0` of a specific call: "0x28 is the `$a0`
  of the call at 0x800A2618";
* four `csrc/unk_*.c` units declare `s32 rand(void);` for themselves.

An argument at a `rand` call site sets `$a0` before the `jal`, so it is
load-bearing for the match wherever the target sets that register. If `rand` is
ever given a project-wide declaration it needs a per-site `__asm__` alias for
the argument-passing sites -- the same device this project already uses for
arity mismatches -- and every affected unit re-gated.


## `rand` -- fixed PER UNIT, 2026-11-26, and the global fix is still wrong

**61 dng units called `rand` with no declaration in scope**, so gcc gave it
`int (...)` in every one of them. Five separate agents hit this and each fixed
it locally for its own unit, which is five agents spending effort on one
mechanical line.

`s32 rand(void);` is now declared in all 61 (the four units live agents were
holding at the time were skipped and will pick it up through their own
hand-backs). **Measured free: `make check` clean, five containers
byte-identical, 6556 functions and 269 stubs unchanged.** The per-unit
measurements each agent took predicted that.

**The global fix remains wrong and this is the second time it has been
checked.** Putting `s32 rand(void);` in `common.h` breaks the build, because
`csrc/ovl_dungeon_800C2824.c:1525` genuinely calls it WITH an argument:

    *(u16 *)(p + 0x2E) = *(s16 *)(p + 0x2E) + 0x40 + (rand(ii) & 0x1F);

That is the only such site in the tree -- the apparent hits in
`csrc/ovl_town.c` are `rand(0xB4)` and similar inside prose comments, not code.
So one real call site blocks a shared declaration for the whole project, and
the per-unit form is the only one available.

**What is left in `declcheck`** on the units that still report warnings is the
`incompatible pointer type` family on GTE arguments (`CompMatrix`,
`RotTransPers`, `RotTransPers4`): the committed resident donors pass
`&D_...` and `&sp->field` to `MATRIX *` / `SVECTOR *` / `long *` parameters and
build byte-identical, so mirroring them is correct and inventing `(long *)`
casts in a tree that does not use `long` is not.
