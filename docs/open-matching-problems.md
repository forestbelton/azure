# Open matching problems

Eleven functions are parked. Every one of them reproduces the original logic;
what is left in each case is a register assignment, the order of two adjacent
instructions, or one dead instruction. The two tie-break rules that explain
most of this class are in `docs/compiler-version.md` ("The two tie-breaks that
decide register choice and instruction order"), and cc1psx's own dumps are the
way to settle them -- see "Reading the compiler's own dumps" in the same file.

| function      | unit    | what is left |
|---------------|---------|--------------|
| func_80043674 | unk_14934 | three registers permuted between two loop scalars and the write pointer |
| func_8003F6F4 | unk_12DE0 | one `li 5` that loop-invariant motion hoists and the original did not |
| func_80047200 | unk_14934 | one dead `addiu $a0,$a0,-0x2` the original kept in a delay slot |
| func_80041284 | unk_14934 | the target copies each loaded global into its variable's register; gcc folds the copy away |
| func_80042560 | unk_14934 | two of three accesses to D_80013714 are compiler-split, the third assembler-expanded |
| func_80042640 | unk_14934 | gcc hoists each constant store into the preceding load's delay slot |
| func_80046D64 | unk_14934 | the target copies the parameter out of `$a0` so the struct base can have it |
| func_8003F8F8 | unk_12DE0 | the width/height pair wants one induction variable with a -2 displacement; gcc makes two |
| func_80043A68 | unk_14934 | two temporaries swapped: the target reuses the address chain's register for the element pointer |
| func_80043B4C | unk_14934 | the same function with `\|= 0x8000` instead of `&= 0x7FFF`; same difference |
| func_8003F80C | unk_12DE0 | two parameters swapped between `$s1` and `$s2`; everything else matches |

The near-miss source for each is in the git history of the commit that parked
it, and the shapes already ruled out are in those commit messages. Do not start
from scratch on one of these without reading its commit message first.

`func_8003CAA8` is not a function. It is the unreachable epilogue of `main`,
which splat split off because it follows `main`'s infinite loop, and it sits at
the start of the `unk_102A8` subsegment. Fixing it means extending the `main`
asm subsegment to 0x102c4 and starting the C unit there; it is a spec change,
not a matching problem.

## Functions blocked by the translation-unit problem

32 of the remaining stubs cannot match in their current unit, because the
addressing mode they need is per-compilation -- see
`docs/addressing-and-translation-units.md`. Five reach their globals only
`$gp`-relatively (func_8003E4FC, func_8003F5E0, func_80044618, func_80044698,
func_800448BC) and 27 do both and so cannot match under either `-G`.

Two of the five are cheap: `func_8003F5E0` is the first function of
`unk_12DE0` and sits immediately after the carved-out `gprel_12DD4`, so
extending that subsegment by one function picks it up; `func_80044618` and
`func_80044698` are adjacent to each other inside `unk_14934` and could be
carved out together. Both need a splat spec change, a re-extraction and a
`MASPSXFLAGS_G8` line in the Makefile.

The 27 mixed ones need the real fix from that document: each C file eventually
has to *define* the globals its functions reach `$gp`-relatively, rather than
declaring them `extern`, so maspsx can see the size. That means moving those
objects out of the splat data segments and into C, which is a much larger
change.

## func_8003E188  (unk_11940) -- resolved, kept for the reasoning

Sets a 2-bit field in a byte of the out-of-image table at 0x80013614. The
*only* difference left is the order of two instruction pairs inside the first
basic block; the instruction multiset, the register allocation of the tail,
the branch arms and the delay slots all match.

Target:

      lui    $v1, %hi(D_80013614)        ; = lui $v1, 0x8001
      sll    $v0, $a0, 16
      sra    $v0, $v0, 18
      addu   $v1, $v0, $v1
      andi   $a0, $a0, 0x3
      sll    $a0, $a0, 1
      lbu    $a2, %lo(D_80013614)($v1)   ; = lbu $a2, 0x3614($v1)
      sll    $a1, $a1, 16
      beqz   $a1, .L8003E1B4
       addiu $v0, $zero, 0x1
      addiu  $v0, $zero, 0x3
      ...

The closest source -- address chain, then mask, then load:

    void func_8003E188(s16 arg0, s16 arg1) {
        s32 sh = (arg0 & 3) * 2;
        u8 v = *((u8 *) 0x80013614 + (arg0 >> 2));
        s32 bits = 1;

        if (arg1 != 0) {
            bits = 3;
        }
        *((u8 *) 0x80013614 + (arg0 >> 2)) = v | (bits << sh);
    }

compiles to the same fifteen instructions with the mask pair (`andi`, `sll`)
scheduled *before* the address chain instead of after it. Positions 7 through
15 are identical.

What was established, and is already paid for:

* The table has to be a constant address, not an `extern` array. Declared as
  an array the access costs one instruction more, because the `%lo` goes into
  an `addiu` instead of riding on the load. See
  `docs/addressing-and-translation-units.md`.
* The load must be a statement of its own, assigned to a local. With `|=`
  gcc puts the `lbu` after the branch, where the target has it before.
* `bits = 1; if (arg1) bits = 3;` is right: it puts the `1` in the branch
  delay slot. An `if/else` adds a `j`, which the target does not have.
* decomp-permuter is no use here (relocation mismatch -- see the same doc).
  It ran 1500 iterations against an unreachable target and its best
  candidates were semantically wrong.

Ruled out: every permutation of the three opening statements (only two
distinct schedules come out of the six orderings); hoisting the index into its
own local; `extern u8 D_80013614[]` at sizes 4, 0x100 and unspecified;
`*(ptr + i)` against `arr[i]`; a `u8 *` local for the base (it materialises
the whole constant with `li`+`ori`); compiling at `-G0` and `-G4`.

The shape is rare: two other accesses in the game have it, `D_80010A93` and
`D_80012273`, both just past func_80043568, so whatever fixes this one is
worth about three functions.

---

# Resolved

Resolved on 2026-10-21. The four functions below reproduced the original
logic but not its instructions, each within one to three instructions, and
each difference was a code-generation choice that could not be steered from C.
They looked like one problem rather than four, and they were: the game was
compiled with the PSY-Q 4.1 `cc1psx` (`cygnus-2.7.2-970404 SN32.3.7.0004`),
not the PSY-Q 4.0 one. See `docs/compiler-version.md` for the evidence and
the toolchain switch. All four now match and `make check` is byte-identical.

What follows is the original write-up with each resolution noted, kept
because the dead ends are worth not retrying and because the shapes recur.

## Toolchain

The build is the two-step pipeline described in the Makefile: `tools/ccpsx`
(wine) runs `cc1psx`, then maspsx emulates ASPSX on top of GNU as:

    tools/ccpsx -O2 -G8 -Iinclude -S -o build/csrc/UNIT.s csrc/UNIT.c
    python3 maspsx.py --aspsx-version=2.56 --force-stdin --run-assembler \
        --gnu-as-path=mips64-linux-gnu-as --use-comm-section -G8 \
        -EL -32 -Iinclude -march=r3000 -mtune=r3000 -no-pad-sections -O1 \
        -o build/csrc/UNIT.o < build/csrc/UNIT.s

`make check` is the only authority: instruction counts can agree while bytes
differ. decomp-permuter is set up (`tools/permuter_compile.sh` is the real
pipeline); after `import.py`, copy that script over the generated
`compile.sh`, which only runs `ccpsx -S` and leaves assembler text at the
`.o` path.

---

## func_8003E14C  (unk_11940) -- resolved

Reads a mode byte and clamps a halfword against 0x29.

Target:

      lui        $v0, %hi(D_80082E6A)
      lbu        $v1, %lo(D_80082E6A)($v0)
      addiu      $v0, $zero, 0x2
      bne        $v1, $v0, .L8003E180
       addu      $v0, $zero, $zero
      ...

Under 4.0 the best C put the `%hi` and the load in the same register. The
`lui`-in-another-register shape is the split-address code generation of the
4.1 compiler, and it only appears for a memory operand. The fix was to read
the byte as a field of the struct it belongs to:

    if (D_80082E60.unk_0A != 2) {
        return 0;
    }

Ruled out at the time, still true: `s16` locals (adds sign extensions), a
single return through a result variable (wrong result register), inverting
the inner comparison (identical output), ~5000 permuter iterations under 4.0
(nothing below the base score, because no source could reach it).

---

## func_800422A8  (unk_14934) -- resolved

Unsigned divide, return the quotient as a halfword or -1. The target branches
on the *failing* comparison to the `-1` arm and lets the sign-extend path fall
through. Under 4.0 every formulation came out with the arms swapped or with
the body restructured (12 instructions instead of 13). Under 4.1:

    if (q >= (u32) (arg3 & 0xFFFF)) {
        return -1;
    }
    return (s16) q;

matches exactly, as does the ternary `return q < x ? (s16) q : -1;`. The
success-first form `if (q < x) return (s16) q; return -1;` still inverts.

---

## func_8003D8B0  (unk_102A8) -- resolved

Zero two buffers, clear four words, three calls. The target materialises the
array base and then adds the 0xC as a separate `addiu`:

      addiu      $v1, $zero, 0x3
      lui        $v0, %hi(D_80083110)
      addiu      $v0, $v0, %lo(D_80083110)
      addiu      $v0, $v0, 0xC

Every pointer form folds the 12 into the `%lo`, under 4.0 and 4.1 alike,
because `(u32 *) &D_80083110 + i` goes through `addsi3` and cse folds it.
An array reference is different under 4.1: `D_80083110[i]` forms the address
as `(lo_sum base) + i*4`, strength reduction initialises the pointer from
the `lo_sum` register, and nothing can fold a constant into a `lo_sum`. So
`structs.h` now declares `extern u32 D_80083110[4];` and the loop is

    for (i = 3; i >= 0; i--) {
        D_80083110[i] = 0;
    }

---

## func_8003FE78  (unk_12DE0) -- resolved

Zero a node, push it on the list head, set flags, compute offsets into the
tail. Two things were wrong under 4.0, and the first was also wrong under 4.1:

1. `s32 f = flags | 0x80;` lets cse fold the later `f | 0x4000` through it
   into `ori $v0, $s2, 0x4080`, which also keeps the raw `flags` live in an
   extra callee-saved register. Updating the parameter in place
   (`flags |= 0x80;`) blocks the fold: cse cannot record an equivalence for a
   register in terms of its own previous value.
2. The target's prologue computes `ori $s1, $a0, 0x80` *before* copying the
   node pointer to `$s0`, and that order is not scheduling: it survives
   `-fno-schedule-insns -fno-schedule-insns2`. The parameter copies are
   emitted in parameter order at function entry, so the node pointer's copy
   can only come after the `ori` if the parameter itself is used exactly once
   -- copied into a local after the flags update -- so that combine merges
   the entry copy into that later assignment. decomp-permuter found this
   (as a `new_var2 = n;` temporary) in 101 iterations; the committed source
   writes it as

       flags |= 0x80;
       n = arg1;

The branch structure, including the shared fall-through into the `unk_0C`
store from two predecessors, reproduced correctly throughout.
