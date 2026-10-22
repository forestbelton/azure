# Open matching problems

Ten functions are parked. Every one of them reproduces the original logic;
what is left in each case is a register assignment, the order of two adjacent
instructions, or one dead instruction. The two tie-break rules that explain
most of this class are in `docs/compiler-version.md` ("The two tie-breaks that
decide register choice and instruction order"), and cc1psx's own dumps are the
way to settle them -- see "Reading the compiler's own dumps" in the same file.

| function      | unit    | what is left |
|---------------|---------|--------------|
| func_80041284 | unk_14934 | arm 1 wants the `*p` reload *after* the store but the store between `lui` and `and`; see below |
| func_80042560 | unk_14934 | addressing solved; loop 1's store/load order and three `$s` registers are not; see below |
| func_80042640 | unk_14934 | gcc hoists each constant store into the preceding load's delay slot |
| func_8003F8F8 | unk_12DE0 | the width/height pair wants one induction variable with a -2 displacement; gcc makes two |
| func_80043A68 | unk_14934 | two temporaries swapped: the target reuses the address chain's register for the element pointer |
| func_80043B4C | unk_14934 | the same function with `\|= 0x8000` instead of `&= 0x7FFF`; same difference |
| func_8003F80C | unk_12DE0 | two parameters swapped between `$s1` and `$s2`; everything else matches |
| func_80042900 | unk_14934 | one extra `move $v0,$zero`: the dead load in the 0xD arm wants `$v1`; see below |
| func_80042A80 | unk_14934 | one bit: `srl` where the target has `sra`; see below |
| func_80047054 | unk_14934 | every instruction identical, five registers permuted; see below |

`docs/near-misses/` holds the source that got each one that close, with the
shapes already ruled out in comments. Read that file before starting on one;
the commit that parked it has the reasoning, but only the file has the source.
Nothing in `docs/near-misses/` is compiled -- the functions there are still
`INCLUDE_ASM` stubs and `make check` does not look at them.

`func_8003CAA8` is not a function. It is the unreachable epilogue of `main`,
which splat split off because it follows `main`'s infinite loop, and it sits at
the start of the `unk_102A8` subsegment. Fixing it means extending the `main`
asm subsegment to 0x102c4 and starting the C unit there; it is a spec change,
not a matching problem.

## func_80041284  (unk_14934) -- partial

Both copies and the empty `jal func_8003F320` delay slot come out right with
the assignment written as a chained store whose value is wanted:

```c
    if (x == 0) {
        x = D_80081480 = D_8008148C;
        y = x + ((*p & 0xFF800000) >> 12);
    } else {
        x |= 0x80000000;
        y = D_80081480 = D_8008148C;
    }
```

`store_expr` with a MEM target and `want_value` does `copy_to_reg`, so the RTL
is load-to-temp / store / copy. combine cannot fold the load into the copy
because the store sits between them (`use_crosses_set_p` via `mem_last_set`),
sched1 then puts the copy before the store, and local-alloc rewrites the
store's source. Arm 2 matches exactly.

What is left is arm 1's order. The target has `lw $v0,0($s2); lui $v1,0xFF80;
sw $s0` -- the `*p` reload before the store -- and this source emits the store
first. The reload exists only because cse's NOT_TAKEN pass saw the store
invalidate the block-0 load, so in RTL the load must follow the store, and
store-before-load is a true dependence in sched1. Ruled out: every statement
order of the two assignments and the `*p` expression, function- and
block-scope temporaries, `volatile` aliases on `D_8008148C` and both globals,
`volatile u32 *` for `p`, an array declaration for `D_8008148C`,
`-fno-schedule-insns`, `-fno-schedule-insns2`, both, `-O1`, and
`-mcpu=r3000/r4000/r4600/r6000`. Note `csrc/unk_14934.c` already uses
`D_80081480 = D_8008148C;` in four matched places, so a declaration change for
either symbol would risk those.

## func_80042560  (unk_14934) -- partial

The mixed addressing is solved, and it really is mixed within the one
function: a constant base pointer for the first two accesses and the
`extern u16 D_80013714` scalar for the third.

```c
    u8 *base = (u8 *) 0x80010000;
    saved = *(u16 *) (base + 0x3714);
    *(u16 *) (base + 0x3714) = saved | 2;
    ...
    D_80013714 = saved;
```

That gives `lui $v1,0x8001 / lhu $s4,0x3714($v1) / ... / sh $v0,0x3714($v1)`
for the first two and the assembler-expanded `lui $at / sh $s4,0x3714($at)`
for the third, matching all five instructions. (`*(u16 *) 0x80013714` gives
`lui+ori` plus `0($v1)`; an `extern u16 D_80013714[]` declaration splits all
three.)

Two things are left. Loop 1's order: the target is `sb $v0,0($s0);
sb $v1,1($s0); lb $a1,0($s0)`, and sched1 instead fills the load delay of
`lb $a1` with the independent `sb $v1`. For the target's order the store would
have to depend on the load, and gcc 2.7.2's `memrefs_conflict_p` never finds a
dependence between offsets 0 and 1 off the same base pseudo (the
`MEM_IN_STRUCT` exception in `true_dependence` needs one operand to be a
non-struct, non-varying reference). Ruled out: `struct {s8 a,b;} tmp[4]`, a
byte pointer into it, `s8 tmp[4][2]`, the stores swapped, no temporaries
(which also turns the loads into `lbu`), an `s8` temporary, a `j = i * 2`
index variable, `volatile s8` (which does create the dependence but turns the
loads into `lbu/sll/sra`), the `-mcpu` set above, and `-mips2`.
`-fno-schedule-insns -fno-schedule-insns2` does reproduce the loop exactly,
but the target's prologue is visibly interleaved by sched2, so the schedulers
were on. Second, the registers: the target has tmp-pointer `$s0`, counter
`$s1`, walker `$s2`; this source gives the walker `$s0` and the counter `$s2`.
The `.greg` priorities are loop-2-tmp 2.45, walker 1.80, loop-1-tmp 1.74,
counter 1.24, so the walker giv has to drop below the counter.

## func_80042900  (unk_14934) -- partial

One instruction too many. The target's `arg1 == 0xD` arm is a single
`lw $v1, 0x14($a0)` whose result is never used -- the function returns 0 on
that path, from the `addu $v0, $zero, $zero` that reorg hoisted into the
`bne`'s delay slot. So the original really did load a word and discard it.

A non-volatile dead load is deleted outright by `flow.c`'s `insn_dead_p` (the
whole arm disappears), so the read has to be volatile. gcc then keeps it but
gives it `$v0`, the first register in `REG_ALLOC_ORDER` and free because the
return value's `$v0 = 0` is only set afterwards, and re-emits `move $v0, zero`
after the load. That move is the extra instruction. Matching it means making
the return value live across the load; the shapes tried are listed in
`docs/near-misses/func_80042900.c`.

## func_80042A80  (unk_14934) -- partial

One bit. Every instruction is identical except the last, where the target has
`sra $v0, $v0, 16` and this has `srl $v0, $v0, 16`.

gcc expands a cast to `s16` as `sll 16` + `sra 16`, and combine's
`simplify_shift_const` downgrades the `sra` to `srl` whenever it can prove bit
15 of the operand is clear. The `andi $v0, $v0, 0x400` immediately before it
proves exactly that, so every spelling of the cast gets `srl`; `andi 0xC000`
and `ori 0x400` both do give `sra`, which confirms it is the proof and not the
spelling. The way out is in combine.c's `nonzero_bits` REG case: it consults
the recorded bits only when `reg_last_set_mode == mode` and
(`reg_n_sets == 1` or `reg_last_set_label == label_tick`), so the mask's set
has to land in a different basic block from the extension, or under a different
mode. No natural source shape found so far does that while still emitting the
two adjacently, which is what the target shows.

Two things here are settled and should not be re-derived: `D_80082E6A` has to
be read as `D_80082E60.unk_0A`, and the dispatch has to be an if-chain -- a
`switch` on 4/0xF/0x10/0x11 makes gcc build a balanced tree of `slti`s instead
of the target's `== 4` followed by `addiu -0xF` / `sltiu 3`. `D_80073414` is a
0x14-byte-stride table and `D_80073470` is `&D_80073414[4].unk_0C`, so the
`case 4` arm needs no separate symbol.

## func_80047054  (unk_14934) -- partial

Every opcode, immediate, displacement and `nop` is identical; five registers
are permuted. The target keeps the first parameter in `$a0` and puts the outer
walker in `$v1`, while this source copies the parameter out (`move $t2, $a0`)
and gives the walker `$a0`; the rest follows.

The `.greg` dump explains it. Allocation order is the n+6 induction variable
(26 refs / 22 insns), `n` (23/28), the outer induction variable (9/36), then
`e` (9 refs / 40 insns) and only then `p` (6 refs / 88 insns), because
`global.c` ranks by roughly `log2(refs) * refs / live_length`. `e` at ~0.68
beats `p` at ~0.14, and `e` additionally carries a preference for `$a0` that
`p` does not have. For the target's assignment `e` would have to be allocated
after the n+6 induction variable -- 0.68 against 4.7, too wide to close by
adjusting reference counts -- or else conflict with `$v1`.

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
