# Which compiler built the game

The game code of SLUS-00614 was compiled with the `cc1psx` that ships on the
PSY-Q **Runtime Library 4.1** disc (DTL-S2330), which reports

    cygnus-2.7.2-970404 SN32.3.7.0004

and not with PSY-Q 4.0's `GNU C 2.7.2.SN32.3.7 Build 0001` that this project
used until 2026-10-21. The libraries linked into the game are still PSY-Q
4.0's; only the compiler is newer. Both facts are measured, not inferred, and
the measurements are recorded here so nobody re-opens the question.

## The symptom

184 absolute global loads in the game pair a `lui` with a load through it.
111 use the destination register as the base (`lui $v0` / `lw $v0, %lo()($v0)`),
which is what any assembler produces when it expands a symbolic `lw`. The
other 73 do not:

    lui   $v0, %hi(D_80082E6A)
    lbu   $v1, %lo(D_80082E6A)($v0)      ; different register

and in func_80041AE4 the `lui` is hoisted across a `jal` and shared:

    jal   func_8003E2D8
     lui  $s1, %hi(D_80082E60)
    addiu $s0, $s1, %lo(D_80082E60)
    lbu   $a0, 0xB($s0)
    ...
    lw    $v0, %lo(D_80082E60)($s1)

No assembler can allocate `$s1` or move an instruction across a call. The
compiler emitted the `lui` and the `%lo` displacement as separate operations
-- gcc's split-address (HIGH / LO_SUM) code generation.

## Why it cannot be PSY-Q 4.0

`strings` on the two binaries settles it. The 4.0 `cc1psx.exe` contains no
`lui ... %hi` output template at all (its only `%hi` is the `-mabicalls`
`%neg(%gp_rel())` prologue), so it physically cannot produce the shape. The
4.1 binary contains

    lui	%0,%%hi(%1) # high
    addiu	%0,%1,%%lo(%2) # low

The 4.1 compiler compiled every function already matched under 4.0 to the
same bytes: `make check` stays byte-identical with 4.0's and 4.1's `cc1psx`
alike, under both the ASPSX 2.56 and 2.67 emulation in maspsx (4.1 ships
ASPSX 2.67; the project keeps 2.56 until something distinguishes them).

PSY-Q 4.4's `cc1psx` (gcc 2.8.1 SN32) also produces the split shape, but it
emits duplicated `jr $ra` returns and puts the stack adjust in the `jr` delay
slot, neither of which appears anywhere in the game. 4.4 is not it.

## The libraries are still 4.0's

`azure libmatch` over the 4.0, 4.1, 4.4 and 4.5 library sets:

    release            exact   fuzzy    unscored
    4.0            162/163     0.998          3
    4.1             98/163     0.799          3
    4.4             98/160     0.804          6
    4.5             98/160     0.803          6

and `azure libscan` locates 166 modules with the 4.0 libraries against 102
with 4.1's. So the splat library layout derived from 4.0 stands; a studio
that updated its SN compiler without replacing Sony's runtime is unremarkable.

## What the switch resolved

The four functions in `docs/open-matching-problems.md` were all this. Three
of them matched on the first compile under 4.1 or with a source-shape change
that only matters because of split addresses:

| function      | 4.0 symptom                              | resolution under 4.1 |
|---------------|------------------------------------------|----------------------|
| func_8003E14C | `lui`/`lbu` in the same register         | access the mode byte as a field of `D_80082E60` |
| func_800422A8 | branch arms the wrong way round          | `if (q >= x) return -1; return (s16) q;` |
| func_8003D8B0 | `+0xC` folded into `%lo`                 | `extern u32 D_80083110[4]` and index it |
| func_8003FE78 | `flags | 0x80` folded into `ori 0x4080`  | `flags |= 0x80;` then copy the node pointer into a local |

The last one took decomp-permuter 101 iterations once the compiler was right.

## How the compiler behaves

Learned while matching, worth knowing before reading m2c output:

* A memory operand on a symbol that is not small data is split into
  `lui` + `%lo` displacement. `sym + const` inside a memory operand becomes
  `(lo_sum base sym) + const`, so the constant is a displacement on the load,
  not part of the `%lo`. That is why struct fields read `0xB($s0)` off a
  register holding the struct's address.
* A plain address (`la`) of a symbol is still emitted as `la` and expanded
  by the assembler; the split only happens for memory operands.
* Hence array reference and pointer arithmetic are no longer equivalent.
  `arr[i]` forms `(lo_sum) + i*4`, and strength reduction leaves the initial
  `+12` as its own `addiu`; `*(ptr + i)` forms the sum through `addsi3` and
  the constant folds into `la sym+12`.
* The `lui` half is a loop invariant and a CSE candidate, so one `lui` serves
  every access to symbols sharing a 64 KB page within a function.
* Which of the two shapes you get is decided by the *declaration*: a scalar or
  an array within the `-G8` limit stays symbolic and is expanded by the
  assembler, anything larger is split by the compiler. That table, and the
  separate case of globals outside the loaded image (constant addresses, not
  symbols), are in `docs/addressing-and-translation-units.md`.

## Source shapes that decided matches

Beyond the four write-ups in `docs/open-matching-problems.md`:

* Where the target keeps an intermediate product in a scratch register, the C
  needs an explicit temporary: `t = v * 10; v = t * n;` keeps `t` in `$v0`,
  while `v = v * 10 * n;` writes the product back into `v`'s register and also
  reassociates the multiply (func_800438E4).
* Grouping decides which operand a constant folds into. `t = (x & m) - 0x20;
  return t + y;` subtracts from the masked value as the target does;
  `(x & m) - 0x20 + y` folds the `- 0x20` into `y` instead (func_800445E0).
* Statement order survives into the schedule. Two adjacent increments of
  struct fields come out in the order the statements are written, even though
  both loads are hoisted above both stores (func_8003E70C).
* A constant offset into a struct stays a load displacement only if it is
  written as part of the index expression: `arg0[i * 2 + 0x2C]` keeps `0x2C`
  on the load and strength-reduces `arg0 + i * 2`, while
  `p = arg0 + 0x2C; p[i * 2]` folds `0x2C` into the initial pointer
  (func_80042518).
* Reloading a global instead of keeping it live is forced by reading it into a
  separate local for the comparison and reading it again for the result; gcc
  rematerialises the load rather than preserving the register
  (func_8003F270).
* An element pointer needs its own local where the target schedules the
  table's address *after* the index multiply. `D_8006CE80[i].field` puts the
  table address first; `e = &D_8006CE80[i]; e->field` puts it after
  (func_80040FDC).
* Several slots in one out-of-image region share a single `lui` only if the C
  reaches them through a constant *base* pointer plus displacements
  (`base = (u8 *) 0x80010000; base[0x3184]`). Three independent constant
  addresses each assemble to their own `lui`/`$at` pair, and three separate
  `extern` symbols get a `lui` each because gcc cannot know they share a
  64 KB page (func_800439F8).

## The two tie-breaks that decide register choice and instruction order

When the arithmetic is right and only the register numbers or the order of two
adjacent instructions differ, it is one of these. Both were read out of gcc
2.7.2's own sources and confirmed against cc1psx's RTL dumps.

**The pre-reload scheduler (`sched.c`).** Every insn has priority 1, except
one that *births* a single-set pseudo, which is bumped to maximum priority;
`reg_n_sets == 1` is the test. So splitting a value's computation over two
statements makes its pseudo multi-set, drops it back to priority 1, and leaves
it in source order -- and that decides which insn `reorg` later finds nearest
for a branch delay slot. func_8003E188 needs `sh = arg0 & 3; sh *= 2;` for
exactly this reason: as one expression the shift is single-set and high
priority, the `bits = 1` sinks below it, and reorg fills the delay slot with
the shift instead of the `li`.

**Local vs global allocnos (`local-alloc.c`, `global.c`).** A variable declared
*inside a block* is a block-local quantity, and local-alloc assigns it before
the block's other values -- by priority, so the one with the most references
takes the lowest hard register. A variable declared at function scope and set
in several blocks becomes a global allocno instead, allocated after local-alloc
has already handed out the low registers, so it lands higher. func_800439F8
needs a separate `base` local inside each arm for this: one function-scope base
comes out in `$a1` where the target has `$v0`. As a bonus, two arms that then
begin with the same `lui` let reorg hoist one copy into the branch delay slot
and delete both, which is what the target shows.

Raising a variable's reference count moves it down the register file too:
func_80043868 needs `a = cur - a` and `d = (a * 20) / d` rather than fresh
locals, and func_8003E240 needs `r &= 0xFFFF; r |= ...` rather than one
expression.

## Reading the compiler's own dumps

`cc1psx` honours gcc's dump flags -- `-dS` (post-sched1), `-dR` (post-sched2),
`-dl` (local-alloc), `-dg` (global-alloc), `-dr` (initial RTL). The files land
in the wine temp directory,
`~/.cache/azure-psyq/drive_c/users/case/AppData/Local/Temp/`, as `PQ*.sched`,
`.sched2`, `.lreg`, `.greg` and so on. The `.sched` dump prints the ready list
and each insn's priority per cycle, and `.greg` prints each allocno with the
hard registers it conflicts with; between them they explain any ordering or
register difference without guessing.

## Installing it

The compiler is not in the repository (`psy-q/` is ignored). It comes from
track 1 of the 4.1 disc image, which is MODE2/2352: strip each sector to its
2048 data bytes, then unpack the ISO.

    python3 - <<'EOF'
    src = "PlayStation Programmer Tool - Runtime Library Version 4.1 (Japan) (Track 1).bin"
    with open(src, "rb") as f, open("track1.iso", "wb") as o:
        while (s := f.read(2352)):
            o.write(s[24:24 + 2048])
    EOF
    7z x track1.iso COMPILER PSSN

Copy `COMPILER/CC1PSX.EXE`, `CPPPSX.EXE` and `CC1PLPSX.EXE` over
`psy-q/compiler/` (the 4.0 copies are kept in `psy-q/compiler-4.0/`), and
`PSSN/ASPSX.EXE` to `psy-q/psyq/aspsx-2.67.exe` for reference. The 4.0
`ccpsx.exe` driver stays; it only forwards options.

The 4.1 disc image lives in `~/Downloads/Programmer Tool - Runtime Library
Version 4.1 (Japan)_DTL-S2330_redump/`.
