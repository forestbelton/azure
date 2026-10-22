# Open matching problems

Eight functions are parked. Every one of them reproduces the original logic;
what is left in each case is a register assignment, the order of two adjacent
instructions, or one dead instruction. The two tie-break rules that explain
most of this class are in `docs/compiler-version.md` ("The two tie-breaks that
decide register choice and instruction order"), and cc1psx's own dumps are the
way to settle them -- see "Reading the compiler's own dumps" in the same file.

| function      | unit    | what is left |
|---------------|---------|--------------|
| func_80042560 | unk_14934 | addressing solved; loop 1's store/load order and three `$s` registers are not; see below |
| func_80042900 | unk_14934 | one extra `move $v0,$zero`: the dead load in the 0xD arm wants `$v1`; see below |
| func_80042A80 | unk_14934 | one bit: `srl` where the target has `sra`; see below |
| func_80047054 | unk_14934 | every instruction identical, five registers permuted; see below |
| func_800423C0 | unk_14934 | 4 of 72: gcc shares one `lui 0x8001` the target materialises three times |
| func_8003D0F0 | unk_102A8 | 212 of 222: the four `(s8)` byte conversions fold to `lb`; see below |
| func_80041E70 | unk_14934 | 4 of 270: cse folds `p->unk_87 * 2` to a constant, and the three register fields follow from that |
| func_800407C0 | unk_12DE0 | 62 of 178, all register fields; argued unreachable -- see the near-miss file |

## func_8003D0F0  (unk_102A8) -- partial, 10 words short

Eight of the ten are the four `(s8)` byte conversions in the flag arms. The
target has `lbu` + `sll 24` + `sra 24` in each; every spelling collapses to a
single `lb`, because combine reduces
`(sign_extend (subreg (zero_extend (mem:QI))))` to `(sign_extend (mem:QI))`.
For the target's form the zero-extended value would need a second use or an
intervening memory *store*, and the target has neither. This is the byte-load
counterpart of the `lh` fold recorded for func_8004425C, so whatever blocks it
is worth more than this one function.

The other two are a copy-then-shift-in-place in the post-call block that
combine folds away. The remaining 190 differing words are a single global
`$s0`/`$s1` swap that should fall out once the byte conversions are right,
since they change the reference counts in four basic blocks.

`docs/near-misses/func_8003D0F0.c` has the source and the eleven shapes that
are already established -- the scratchpad pointer local, the deliberately
literal matrix arguments, the separate `sz` local, and the rest.

## How much of this list is still reachable from C

Taking stock after a session that unparked five of the original eleven
(func_80043A68, func_80043B4C, func_8003F80C, func_80042640, func_8003F8F8 and
func_80041284): what is left divides into two kinds, and the division matters
for where effort goes next.

**Reachable, mechanism known.** `func_8003D0F0` -- eight of its ten words are
the `(s8)` fold, and the fix is now measured: any second use of the loaded byte
blocks it. The task is to find that second use in the original.

**Probably not reachable from C**, each for a reason now written down rather
than guessed:

| function | the pass that owns it |
|----------|-----------------------|
| func_80042A80 | `combine` proving bit 15 clear through `nonzero_bits`; twelve shapes measured against it |
| func_80041E70 | `cse` folding a known constant, which `rtx_cost` guarantees it prefers; ten shapes measured |
| func_800407C0 | a call's argument preferences pruning every other preference around it |
| func_80047054 | two allocnos sharing a `$a0` preference with a five-fold priority gap |
| func_80042560 | `sched1` would need a dependence between two disjoint byte offsets, which `memrefs_conflict_p` can never produce |
| func_80042900 | the register for a dead volatile load, i.e. another allocation preference |
| func_800423C0 | `cse` sharing one `lui` across an extended basic block |

That is the honest reading: the parked list is close to exhausted as far as C
can take it, and the remaining effort belongs on the jump-table spec change and
the addressing decision rather than here. `docs/matching-levers.md` section 10
collects the dead ends so they are not retried.

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

## Functions blocked by jump-table placement

Four functions dispatch through a jump table, and all four are blocked:
`func_80041344` and `func_80041588` (unk_14934, 145 and 164 instructions),
`func_80042BDC` (unk_14934, 543) and `func_8003E758` (unk_11940, 698 -- also
in the `$gp` group). That is about 1550 instructions, the largest blocked
group after the translation-unit problem.

The tables live inside the **opening** `[0x800, bin]` subsegment (vram
0x8002D000..0x80032E60), data that sits before all the code in the image.
Compiling a switch does emit gcc's own table into `.rdata`, with the right
entries in the right order, but `build/slus_006.14.ld` collects every
`.rodata` at `_800_RODATA_START`, far past `.text`, so the table lands nowhere
near the right address and the image shifts.

### What the table bytes say about the original translation units

Reading the region out of `data/slus_006.14` pins the layout down exactly:

| symbol | vram | entries | targets | owner |
|--------|------|---------|---------|-------|
| jtbl_8002D5C0 | 0x8002D5C0..0x8002D62F | 28 | 0x8003EE60..0x8003F1F8 | func_8003E758 |
| jtbl_8002D630 | 0x8002D630..0x8002D653 | 9 | 0x800413A0..0x80041528 | func_80041344 |
| *(pad)* | 0x8002D654 | 1 zero word | | |
| jtbl_8002D658 | 0x8002D658..0x8002D67B | 9 | 0x8004164C..0x800417B0 | func_80041588 |
| jtbl_8002D67C | 0x8002D67C.. | 13+ | 0x80042C2C..0x80043430 | func_80042BDC |

The tables are contiguous except for that one zero word, and its position is
informative. 0x8002D5C0, 0x8002D630 and 0x8002D658 are all 8-byte aligned
while 0x8002D654 and 0x8002D67C are not, so `.rdata` was aligned to 8 per
translation unit. func_80041344's table therefore **ends a translation unit**
and func_80041588's **begins the next one**, while func_80041588's and
func_80042BDC's are contiguous and so belong to the same unit.

So `unk_14934` as currently split lumps at least two original units together,
with a boundary at func_80041588 -- and that boundary has to exist in the C
before the tables can land, because one unit's `.rdata` would otherwise emit
all three tables back to back with no pad at 0x8002D654.

### What landing them needs

1. `csrc/unk_14934.c` split in two at func_80041588, with matching splat
   subsegments, so each original unit gets its own `.rdata`.
2. The opening bin blob carved so 0x8002D5C0..(end of jtbl_8002D67C) is not
   also emitted as raw data.
3. Linker-script placement of those three units' `.rodata` at 0x8002D5C0,
   0x8002D630 and 0x8002D658, with 8-byte alignment between sections so the
   pad word falls out on its own.
4. All three unk_14934 functions decompiled **at once**: a missing one leaves
   its unit's `.rodata` short and everything after it shifts, so they cannot be
   landed one at a time.

`func_8003E758` additionally needs the `$gp` work from
`docs/addressing-and-translation-units.md`, so it comes last.

Note that `verify.py` cannot see any of this -- it masks the relocated
halfwords that reference the table -- so a MATCH there is not evidence and only
`make check` is the authority. `docs/near-misses/func_80041344.c` has the
source that reaches 145/145 words and 67 DIFF, and the register-allocation
analysis of what would still be left.

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

Two things are left, and the first is now believed unreachable. Loop 1's
order: the target is `sb $v0,0($s0);
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
were on. Reassessed: the target's order needs `sb 1` to be
unable to move past `lb 0`, i.e. a dependence between two one-byte accesses at
disjoint constant offsets off the same base. `memrefs_conflict_p` can always
prove those disjoint, and `true_dependence`'s differing-flags exception only
ever removes dependences, never adds one, so no spelling of the two statements
can create it. See docs/matching-levers.md section 10. Second, the registers: the target has tmp-pointer `$s0`, counter
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

## func_800423C0  (unk_14934) -- partial

Four instructions out of 72. Everything up to and including the
func_800A1BD0 call is exact, as are all four accesses through the indexed base
and the whole tail. What is left is how three 0x8001xxxx constants reach
registers: the two `addu` operands are swapped, and where the target
materialises a third independent `lui 0x8001` for 0x80012096, gcc reuses the
base pointer's register (`move $a1, $v0`). gcc splits a large constant into
lui + ori as two RTL insns, so cse shares the `lui 0x8001` across the extended
basic block, and the 0x80012096 arm is the `bnez`'s fall-through -- inside it.

The part worth not re-deriving is the addressing. The indexed base is the
plain constant 0x80010000 with 0x2094/0x2098 as load displacements, not the
symbol D_80012094, even though the listing reads `lui $v1, %hi(D_80012094)` /
`lhu $a0, %lo(D_80012094)($v1)`: splat renders a bare `lui reg, 0x8001` as
`%hi(SYM)` whenever it can pair the register with a later `%lo`. A symbol
array with a *variable* index always materialises the whole address with
`lui %hi` + `addiu %lo` and puts `0(reg)` on the load -- measured for an
incomplete array, a complete `u16 B[0x100]`, two accesses at different
indices, and a `union { u16 h; u8 b; }` element type. Only the constant base
pointer gives the target's form.

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
