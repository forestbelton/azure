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
