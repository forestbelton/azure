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

* Writing a store's destination first in the statement decides which address
  cse anchors on. `D[i] = f(...) + D[i-1];` expands the destination address
  before the RHS, so `lo_sum(D)` enters cse's table first and `D-4` becomes its
  related value; `v = f(...) + D[i-1]; D[i] = v;` anchors both induction
  variables on `%hi(D-4)` and costs an extra `addu` (func_80043674).
* Indexing a global array (`D[i]`) rather than walking a pointer changes the
  register assignment: both become strength-reduced induction variables, but
  their pseudos are born later and carry different reference counts
  (func_80043674, func_8003F6F4, func_80046D64).
* A loop increment placed *first* in the body is stolen by reorg's
  `fill_slots_from_thread`: the back-edge branch finds it at its target, copies
  it into its own delay slot, redirects past it, and emits a compensating
  negated add on the fall-through path. That is where an apparently dead
  `addiu $reg,$reg,-N` after a `j` comes from -- it is not a kept dead
  instruction (func_80047200).
* A copy out of an argument register survives only if the value is used twice
  and its consumer is a loop variable: then combine cannot fold the copy and
  local-alloc cannot tie the two (one is a global allocno), so the `addu`
  remains and every allocno after it is renumbered (func_80046D64).
* `abs()` is a gcc 2.7.2 builtin *once declared*, and its MIPS `abssi2` pattern
  emits `bgez / addu / negu` -- a branch over a move-then-negate that no C
  conditional can reproduce, because any visible copy lets cse canonicalise
  either the compare or the negation to the other register (func_8003F6F4).
* A chained store whose value is wanted (`x = G = H;`) goes through
  `store_expr`'s `copy_to_reg`, giving load / store / copy. combine cannot fold
  the load into the copy across the store (`use_crosses_set_p`), so the copy
  survives and sched1 lifts it above the store (func_80041284).
* `move_movables` hoists a loop-invariant `li` only while
  `threshold * savings * lifetime >= insn_count`; making the loop body bigger
  leaves the constant in place (func_8003F6F4).

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
`global-alloc` runs at all. **How it orders them was read out of
local-alloc.c on 2026-10-26** (the tree is at scratch/gcc-src), and the short
version in this document used to be wrong in a way that cost a wrong write-up,
so here it is exactly.

`block_alloc` allocates in TWO PHASES, and the first one ignores priority:

1. **The suggestion phase.** `combine_regs` records a SUGGESTED hard register
   for any quantity that is one side of a move whose other side is a hard
   register -- a call's return value, or a value copied into an argument
   register. Plain moves go in `qty_phys_copy_sugg`, everything else in
   `qty_phys_sugg`. The quantities are then sorted by `qty_sugg_compare_1`,
   which orders by the NUMBER of suggestions first ("most restrictive needs
   first") and only then by priority, and every quantity that has a suggestion
   is allocated immediately with `find_free_reg (..., just_try_suggested=1)`.
2. **The priority phase.** Everything left is sorted by `qty_compare_1` and
   allocated.

So a quantity born from a copy of a hard register is allocated BEFORE every
quantity that has no such copy, however much higher the latter's priority.
That is the correction: func_8004DE88's two competing quantities are 0.714 and
0.688 by the formula below, and **the margin is irrelevant** -- the cursor wins
because it is born from a copy of `$v0` and the address value is born from an
`addiu`, so they are not even in the same phase.

The priority formula itself, used by both `qty_compare_1` and as the second key
of `qty_sugg_compare_1`, is

    floor_log2(refs) * refs * size / (death - birth)

with the quantity NUMBER as the final tie-break -- so among equals, birth order
decides. `floor_log2` means a quantity needs to cross a power of two in its
reference count to move much: 4 refs and 5 refs give 2 and 2, but 7 and 8 give
2 and 3.

**And a quantity that crosses a call cannot get a call-clobbered register in
the suggestion phase.** `find_free_reg` is called there with
`accept_call_clobbered = 0`, and it starts its `used` set from
`call_used_reg_set` whenever `qty_n_calls_crossed[qty] > 0`. That is why
`tools/decomp/advise.py --greg` prints a `crosses N calls` column: for a
quantity with a `$v0` suggestion, one intervening call is the difference
between taking `$v0` in phase 1 and falling through to phase 2 with the copy
left behind.

**It is a real lever and it is not an explanation for every surviving copy.**
func_8004DE88 was briefly written up as an instance of it and is not one: its
target's cursor crosses no call either, so something else kept that quantity
out of phase 1. Check the listing for a `jal` inside the live range before
reaching for this. A variable declared at function scope and set
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
`-dl` (local-alloc), `-dg` (global-alloc), `-dr` (initial RTL) -- and `-da`
dumps all of them at once, which is what `tools/decomp/dump.sh` passes. The
files land in the wine temp directory, under the prefix `tools/ccpsx` chose
(`$AZURE_WINEPREFIX` if set, else `~/.cache/azure-psyq`), as
`drive_c/users/*/AppData/Local/Temp/PQ*.<suffix>`.

**There are sixteen dumps, and three of them do not exist in upstream 2.7.2.**
Measured from a cleared temp directory, cc1psx emits `rtl jump cse loop cse2
flow combine regmove lreg greg sched sched2 bp dbr shorten`, all headed with the
compiled function's name. Upstream 2.7.2's `toplev.c` knows only `.rtl .jump
.cse .loop .cse2 .flow .combine .lreg .greg .sched .sched2 .dbr .stack`, and
there is **no `regmove.c` in the upstream 2.7.2 tarball at all** -- that pass
arrived later. So `regmove` and `bp` are passes the Cygnus/SN build carries and
upstream does not, and `.stack` is one upstream has that cc1psx does not emit.

Two consequences. First, **upstream 2.7.2 source is a good reference for the
passes it has and no reference at all for `regmove`** -- treat a conclusion that
depends on the pass list as resting on an assumption, in the sense
`docs/where-we-are.md`'s context audit means. Second, `regmove` rewrites
register copies and in later gcc adjusts `REG_N_REFS` and `REG_LIVE_LENGTH`,
which are exactly the inputs to `global.c`'s priority formula that several
near-miss bounds are computed from -- so it looked worth diffing `.combine`
against `.regmove` before trusting such a bound.

**Measured, and it is a no-op.** On func_8003F368, func_80041E70, func_8004437C,
func_800407C0 and func_80047054 the two dumps are byte-identical apart from the
combiner's own statistics header and footer: same insn count, same RTL, same
registers. So regmove explains none of the register permutations, and no bound
computed from `REG_N_REFS` / `REG_LIVE_LENGTH` is disturbed by it. Only the two
functions an agent was holding at the time are untested.

The likely reason, offered as inference rather than fact since there is no
source for this pass: regmove of that era exists to coalesce for **two-address**
targets, making an insn's destination share a register with one of its inputs.
MIPS is three-address, so it has nothing to do. Consistent with a no-op on every
function tested, but not verified.

**So do not decompile cc1psx to recover regmove.** The idea is sound in
principle -- cc1psx is a GCC derivative, its source is GPL, and there is no
upstream 2.7.2 source for this pass -- but the pass demonstrably does nothing
here, and the binary is largely stripped (only `except.c` and `expr.c` survive as
assert strings, and "regmove" appears only as the dump suffix), so it would be
expensive as well as unrewarding. If some future function ever shows a live
`.combine` -> `.regmove` difference, the cheap route is still not the binary:
read `regmove.c` from gcc 2.8.0 or egcs-1.0.x, the trunk era this 1997-04-04
Cygnus snapshot would have backported it from, and only then consider
disassembly.

A trap this cost once: a dump collector that deletes a LIST of suffixes before
compiling leaves the unlisted three behind, and the next run collects a previous
function's dump under the new function's name -- a `.regmove` was collected
headed `;; Function func_80041344` while a different function was being
compiled. `dump.sh` now wipes `PQ*.*` wholesale and resolves the wine prefix
the same way `tools/ccpsx` does, which it previously did not. The `.sched` dump prints the ready list
and each insn's priority per cycle, and `.greg` prints each allocno with the
hard registers it conflicts with; between them they explain any ordering or
register difference without guessing.

## Scheduler aliasing, exactly

gcc 2.7.2's `sched.c` decides memory dependence purely on offsets off the same
base register. Two accesses at offsets 0 and 1 off one base pseudo are never
dependent, so sched1 will fill a load's delay slot with a neighbouring store
and no source shape prevents it; the `MEM_IN_STRUCT` exception in
`true_dependence` is narrower than usually described -- it needs one operand to
be a non-struct, non-varying reference. Symbol-versus-register addresses do
conflict, so a store to a global before a load through a pointer is a real
dependence.

The `-G8` driver passes no `-mcpu`, so cc1psx uses r3000 timings (load latency
2 in the `.sched` traces).

When comparing built bytes against a listing, pass `objdump -z`: without it
objdump elides runs of zero words as `...` and hides the hazard `nop`s maspsx
inserts after `mflo`/`mult`.

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
