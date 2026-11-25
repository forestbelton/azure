# Undeclared callees: 19 symbols across 13 units

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
