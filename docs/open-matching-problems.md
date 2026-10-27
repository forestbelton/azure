# Open matching problems

Five functions are parked, and this file's table is kept in step with
`docs/where-we-are.md` -- that one is the authority. Every one reproduces the
original logic; what is left in each case is a register assignment, the order of
two adjacent instructions, or one dead instruction.

**On the word "unreachable", which appears in a few places below.** These bytes
came out of cc1psx compiling a C file, so a C source that reproduces them
exists. Every unreachability claim here is therefore narrower than it sounds:
unreachable *given the unit boundary, declared symbol sizes, `-G` value, struct
layout and function order this project has inferred*, all of which are
inferences rather than evidence. See the context audit in
`docs/where-we-are.md`; four dead ends in this file turned out to live there. The two tie-break rules that explain
most of this class are in `docs/compiler-version.md` ("The two tie-breaks that
decide register choice and instruction order"), and cc1psx's own dumps are the
way to settle them -- see "Reading the compiler's own dumps" in the same file.

| function      | unit    | what is left |
|---------------|---------|--------------|
| func_80041E70 | unk_154BC | 1 of 270: cse puts `(const_int 16)` in mem(0x87)'s class and `rtx_cost` prefers it over the shift |
| func_8003F368 | unk_12A70 | 4 of 113, in two independent two-word gaps: local-alloc, and one named reorg check |
| func_8004437C | gprel_17B7C | 8 of 100: an indexed symbol's address split between local-alloc and global-alloc |
| func_80047054 | unk_19434 | 30 of 58, every instruction identical and five registers permuted; see below |
| func_800407C0 | unk_12DEC | 62 of 178, all register fields: a hard-reg preference created by the memcpy call -- see the near-miss file |

## func_8003D0F0  (unk_102A8) -- SOLVED 2026-10-22

Landed in `csrc/unk_102A8.c`, 222 of 222 words, with ten load-bearing shapes
commented there.

Kept for the failure mode. This section, and the near-miss header, concluded
that the four `(s8)` conversions needed "either a third blocking mechanism or,
more likely, a different model of the arm's source", on the strength of a probe
that hit an eight-insn floor against the target's seven: both known ways of
blocking combine's refold to `lb` cost a `move` the arm has no room for. The
reasoning was sound and the probe was run with an `s32` accumulator. **With an
`s16` accumulator -- a HImode consumer -- the refold simply does not happen,
for free**, and the statement structure was exactly what the near-miss assumed
all along. Eight words, first try.

The lesson is narrow and worth keeping: a probe that establishes a floor
establishes it only for the types the probe used. Both earlier write-ups
described the blocker in terms of what the source *does* (a store between, a
second use) rather than what combine *needs* (a `memory_operand` in SImode),
and that framing is what hid the third option. See
`docs/matching-levers.md` section 7, which now leads with the mechanism.

## How much of this list is still reachable from C

Taking stock after a session that unparked five of the original eleven
(func_80043A68, func_80043B4C, func_8003F80C, func_80042640, func_8003F8F8 and
func_80041284): what is left divides into two kinds, and the division matters
for where effort goes next.

**SOLVED 2026-10-23: `func_80045CC4` landed**, and so did its sibling
`func_800453E0`. Kept here because this section had the diagnosis right and the
cause wrong twice over, which is the failure mode this file exists to record.

The "0.9% priority gap" was real -- 1.4309 against 1.4438 -- and the fix was not
to raise the scratchpad pointer's priority, as the duplicate-an-insn lever
suggests. It was to stop inflating the GIV's. An `if`/`else` whose arms both
store to the same field charges the base pseudo two loop-weighted references for
a store that exists once in the output, because `reg_n_refs` is counted by
`flow` and jump2 cross-jumps the arms long afterwards. Written as one store the
giv drops from 77 refs to 75, and the scratchpad is allocated first. One
reference was enough.

And the "four-word tension between the mask hoisting and the `or`'s operand
order" was not a tension. Those statements are bitfield stores through a
P_TAG-shaped `struct { u32 addr:24; u32 len:8; }` -- libgpu's `addPrim` -- and
no spelling of a hand-written `|` can satisfy both requirements, because
`expand_binop` materialises operand 0's chain first. The bitfield expansion
satisfies both by construction. Dozens of operand spellings across two headers
had been failing at a C operator the original never used.

Both mechanisms are in `docs/matching-levers.md`; the load-bearing shapes are
commented on the landed functions in `csrc/gprel_18B40.c` and
`csrc/unk_19434.c`.

**Probably not reachable from C**, each for a reason now written down rather
than guessed:

| function | the pass that owns it |
|----------|-----------------------|
| func_80042A80 | `combine` proving bit 15 clear through `nonzero_bits`; twelve shapes measured against it |
| func_80041E70 | `cse`'s run boundary: it ends only at a label with `LABEL_NUSES != 1`, which is also a scheduler block boundary, and the target's first block provably has none. Proved, not assumed -- see below |
| func_800407C0 | a call's argument preferences pruning every other preference around it |
| func_80047054 | two allocnos sharing a `$a0` preference with a five-fold priority gap |
| func_80042900 | one duplicated `move $v0,zero`: the join label is not an `own_thread_p`, so reorg copies its first insn into the delay slot and must leave the original. The register question that used to be here is SOLVED |

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

**All four of the functions this section was written about are now landed, and
the mechanism it derives is the one still in use.** Kept because the derivation
is what makes the placement recipe reusable, and because eight more functions
in `unk_1C540` are waiting on exactly it -- see the region map at the end of
the section.

Four functions dispatch through a jump table, and all four were blocked:
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

| symbol | vram | entries | targets | owner | unit |
|--------|------|---------|---------|-------|------|
| jtbl_8002D5C0 | 0x8002D5C0..0x8002D62F | 28 | 0x8003EE60..0x8003F1F8 | func_8003E758 | unk_11F0C |
| jtbl_8002D630 | 0x8002D630..0x8002D653 | 9 | 0x800413A0..0x80041528 | func_80041344 | unk_14934 |
| *(pad)* | 0x8002D654 | 1 zero word | | | |
| jtbl_8002D658 | 0x8002D658..0x8002D67B | 9 | 0x8004164C..0x800417B0 | func_80041588 | unk_14934 |
| jtbl_8002D67C | 0x8002D67C..0x8002D6FB | **32** | 0x80042C2C..0x80043430 | func_80042BDC | unk_154BC |

**The region is exactly three `.rodata` output sections laid end to end, one
per existing splat unit, and no C file has to be split.** This supersedes an
earlier reading of the same bytes, which took the zero word at 0x8002D654 for
a translation-unit boundary and concluded that `csrc/unk_14934.c` had to be cut
in two at func_80041588, and that func_80041588's and func_80042BDC's tables
shared a unit. Both conclusions were wrong, and the second was impossible on
its face: `gprel_152E4` sits between those two functions in the text, and a
translation unit's code is contiguous.

What actually produces the layout is one fact about cc1psx, verified by
compiling for it rather than inferred from the bytes:

* **cc1psx emits each jump table into `.rdata` under `.align 3`.** A unit with
  two 9-entry switches therefore has a `.rodata` of 0x4C bytes: the first table
  at offset 0, a zero word of padding at 0x24, the second at 0x28. Compiled and
  measured; `csrc/unk_14934.c` as it now stands produces a `.rodata` of exactly
  0x4C.
* **The leading `.align 3` is a no-op**, because it sits at offset 0 of the
  section. So a unit's `.rodata` is 8-aligned *internally* but carries no
  alignment requirement of its own at link time -- and `subalign: 4` on the
  segment overrides the section's recorded 2**3 anyway.

Those two together explain both alignment facts at once, which the
unit-boundary reading could not: the pad at 0x8002D654 is the *second* table's
`.align 3` **inside** unk_14934, and jtbl_8002D67C is free to begin at a
merely 4-aligned 0x8002D67C because it is the *first* table in unk_154BC. The
three runs then tile the region with nothing left over:

```
0x8002D5C0 + 0x70 (unk_11F0C: 28 entries)          = 0x8002D630
0x8002D630 + 0x4C (unk_14934: 9 + pad + 9 entries) = 0x8002D67C
0x8002D67C + 0x80 (unk_154BC: 32 entries)          = 0x8002D6FC
```

splat knows this mechanism itself, and says so in
`segtypes/common/rodata.py`: "GCC aligns jumptables to 8, but it doesn't impose
alignment restrictions for sections themselves on PSX/PS2... We can use this as
a way to find file splits." The warning it prints on a misaligned jump table is
a *hint*, not proof -- here it would have pointed at the wrong boundary.

### What landing them needs

Placement, and nothing else. There is no C split and no linker arithmetic to
do by hand; the whole change is in `splat/slus_006.14.yaml`, and it rests on
three details of splat that are easy to get wrong:

1. **The subsegment type must be written `.rodata`, with the leading dot.**
   `CommonSegData.out_path()` branches on `self.type.startswith(".")`: with the
   dot it returns the sibling C file, so the linker entry becomes
   `build/csrc/<unit>.o(.rodata)` and `should_self_split()` is False, so
   nothing is extracted. Written as plain `rodata` it instead disassembles the
   bytes into `asm/data/<unit>.rodata.s`, which the Makefile's `SFILES` glob
   would then assemble and link *as well* as the C object.
2. **Each placement needs its own splat segment.** Under
   `ld_legacy_generation: True` a segment's entries are grouped by section, so
   one segment can hold only one `.rodata` run, after all of its `.data`. The
   opening blob therefore has to be cut into a segment per run. This is not
   optional cosmetics: the segment's `.rodata` group is also where the
   `str/name/*` Shift-JIS subsegments live (they are `.rodata` too, at
   0x541D0), so adding a `.rodata` subsegment to the existing single segment
   drags *those* to 0x8002D5C0 and destroys the image.
3. **Set `align: 4` on each new segment.** The default segment align is 16, and
   `_end_segment` emits `__romPos = ALIGN(__romPos, 16)` after it. A segment
   ending at 0x8002D67C or 0x8002D6FC would be rounded up to a 16-byte boundary
   and every later byte would shift.

The staged form -- one run at a time, as each unit's switch functions land --
looks like this. unk_14934 is first because both of its functions now match:

```yaml
  - type: code
    start: 0x800
    vram: 0x8002D000
    align: 4
    subsegments:
      - [0x800, bin]                      # jtbl_8002D5C0 still raw in here
      - [0xe30, .rodata, unk_14934]
  - type: code
    start: 0xe7c
    vram: 0x8002D67C
    align: 4
    subsegments:
      - [0xe7c, bin]                      # jtbl_8002D67C still raw in here
      ...the rest of the old segment...
```

A unit's run can only be placed once **every** switch function in that unit is
matching, because the run's length is the sum of its tables: with one of
unk_14934's two functions still `INCLUDE_ASM` the `.rodata` is 0x24 rather than
0x4C and everything after it shifts. unk_11F0C and unk_154BC each hold exactly
one of these functions, so each can be placed on its own as soon as that one
lands.

`func_8003E758` additionally needs the `$gp` work from
`docs/addressing-and-translation-units.md`, so it comes last.

### Case order, recovered from the table bytes

The arms are not in numeric order in the source, and the table is the only
record of what order they were in. Read out of `data/slus_006.14`:

| table | file offset | entries | source order of the cases |
|-------|-------------|---------|---------------------------|
| jtbl_8002D5C0 (func_8003E758) | 0xDC0 | 28 | 0, 6, 9, 21, 27 -- numeric; the other 23 values share the default |
| jtbl_8002D630 (func_80041344) | 0xB30 | 9 | 1, 5, 2, 6, 7, 8, 3, 4, 9 |
| jtbl_8002D658 (func_80041588) | 0xE58 | 9 | 1, 5, 2, 6, 7, 8, 3, 4, 9 |
| jtbl_8002D67C (func_80042BDC) | 0xE7C | 32 | 1..10, 27, 11, 12, 26, 32, (21/22/23), 18, 19, 20 |

**jtbl_8002D67C has 32 entries and 128 bytes, not the "13+" first recorded.**
The range check is `sltiu $v0, $v1, 0x20` and the word at 0x8002D6FC is
0x94826082 -- Shift-JIS text, not a code address. Verified from the listing and
again from the file. 0x80043430 is both the default label and the out-of-range
target, so the eleven values 13-17, 24, 25 and 28-31 have no `case` label in
the source at all, and cases 21, 22 and 23 share one body. **So the region the
linker script has to place runs to 0x8002D6FB.** jtbl_8002D658 and
jtbl_8002D67C are contiguous with no pad between them, which is explained by
jtbl_8002D67C being the *first* table of unk_154BC rather than by the two
sharing a unit -- see the section above.

The first two orders being identical is further evidence the two are siblings
over one command format -- as are their shared `default: return;`, the `blez`
on case 9, the `addu $t0, $s0, $zero` record copy, and the record type itself.
Both are now landed, which settles it.

### State of the four

| function | unit | words | DIFF | source |
|----------|------|-------|------|--------|
| func_80041344 | unk_14934 | 145 of 145 | **MATCH** | `csrc/unk_14934.c` |
| func_80041588 | unk_14934 | 164 of 164 | **MATCH** | `csrc/unk_14934.c` |
| func_80042BDC | unk_154BC | 543 of 543 | **MATCH** | `csrc/unk_154BC.c` |
| func_8003E758 | unk_11F0C | 698 of 698 | **MATCH** | `csrc/unk_11F0C.c` |

**ALL FOUR MATCH AND ALL THREE RUNS ARE PLACED** -- unk_11F0C's at
0x8002D5C0, unk_14934's at 0x8002D630 and unk_154BC's at 0x8002D67C, confirmed
by `make check`, and `jtbl_8002D5C0`, `jtbl_8002D630` and `jtbl_8002D67C` have
all dropped out of `undefined_syms_auto` because the compiler now emits them.
This section is closed; it is kept for the mechanism and the recovered case
orders.

One detail the recipe above needs spelling out, learnt placing the second run:
under `ld_legacy_generation` a segment emits ALL of its `.data` before its
`.rodata`, so a run must be the **only** subsegment of its segment -- a
`[0xe7c, bin]` left ahead of `[0xe7c, .rodata, unk_154BC]` would be placed
first and the table would land 0x5C0 bytes late. The bin therefore moves into
the *following* segment rather than staying put, and `--modes ld bin` is needed
(not just `ld`) so the relocated bin's asset is regenerated. What closed them was one mechanism, and it is worth stating
plainly because two sessions worked around it instead:

* The switch index, the flag word and the loop bounds are **one source
  variable**, with the arms renumbered from `case 0` so `expand_end_case` skips
  its own MINUS and the index is a real variable rather than a compiler temp.
  Applying that to func_80041588 took it from 21 DIFF to a match outright.
* func_80041344 needed one thing more: that variable must also win the
  allocation-order comparison against the shared loop counter, because
  `regs_someone_prefers` makes a higher-priority allocno *avoid* a register
  that a lower-priority conflicting allocno prefers -- and `op` prefers $a3,
  being func_8003F80C's fourth argument. The fix is **two extra references for
  `op` at zero instruction cost**, obtained by splitting case 9's pointer into
  `op = (s32) base; op += e->unk_04;` (one `addu` either way). See the comment
  on the function for the priority arithmetic.

**Its stated remaining lever -- "break `flags`' argument-register preference"
-- was refuted and is not what closed it**: that preference propagates along
the dataflow chain through arithmetic insns, and splitting the chain through
another variable leaves both allocnos carrying it. What works is winning the
allocation ORDER, and the cheapest way to move a priority is a reference the
compiler emits no instruction for.

func_80042BDC is landed, but **its load-bearing shapes are not written up**:
the subagent that closed it was stopped by a usage limit immediately after
reporting the match, so only the source survived. The 14-DIFF near-miss it
replaced -- all in case 10, the one word to chase being an `addu $a0,$s1,$zero`
the target emits twice around a join -- is in git history. Treat the shapes in
`csrc/unk_154BC.c` as unexplained rather than deliberate.

Note that `verify.py` cannot see the placement at all -- it masks the relocated
halfwords that reference the table -- so a MATCH there is necessary and not
sufficient, and only `make check` is the authority on whether a table landed.

### The whole region, and the seven tables still unplaced

Measured 2026-10-26 against `data/slus_006.14`. The `.rodata` region of the
executable is file 0x800..0x6b60 (vram 0x8002D000..0x80033360), and it holds
seven runs of code pointers separated by const data. Scanning for aligned words
in 0x80010000..0x80090000:

| file | vram | words | tables | referencing stub |
|------|------|-------|--------|------------------|
| 0x00dc0..0x00efc | 8002D5C0..8002D6FC | 79 | jtbl_8002D5C0 / D630 / D658 / D67C | **placed** (unk_11F0C, unk_14934, unk_154BC) |
| 0x01df8..0x01e10 | 8002E5F8..8002E610 |  6 | jtbl_8002E5F8 | func_8004CECC (102 w) |
| 0x02164..0x02184 | 8002E964..8002E984 |  8 | jtbl_8002E964 | func_800517CC (268 w) |
| 0x027dc..0x02810 | 8002EFDC..8002F010 | 13 | jtbl_8002EFDC (5), jtbl_8002EFF0 (8) | func_80052144 (292 w), func_80052CE0 (194 w) |
| 0x06614..0x068fc | 80032E14..800330FC |186 | jtbl_80032E14 (36), jtbl_80032EA4 (23), pad, jtbl_80032F04 (126) | func_80054F9C (28 w), func_80057A94 (163 w), func_80057D20 (463 w) |
| 0x0690c..0x069f4 | 8003310C..800331F4 | 58 | jtbl_8003310C | **placed** (unk_1C540, func_800595C0) |
| 0x06a3c..0x06ab8 | 8003323C..800332B8 | 31 | jtbl_8003323C, jtbl_8003325C | none -- referenced by no remaining asm |

Entry counts come from each dispatch's `sltiu` bound, and every run's extent
agrees with those bounds, so the grouping is measured rather than inferred.

**The run order matches the referencing functions' text order exactly**
(func_8004CECC < func_800517CC < func_80052144 < func_80052CE0 < func_80054F9C
< func_80057A94 < func_80057D20 < func_800595C0). That is the signature of six
separate original translation units inside what this project carved as the
single unit `unk_1C540`: a linker lays .rdata out in link order, and link order
is text order.

#### What that implies for placing them

`unk_1C540.o` can supply exactly **one** run, so the first table needed no
unit split: its own `type: code` segment, `align: 4`, one
`[off, .rodata, unk_1C540]` subsegment, with the surrounding `bin` cut either
side, and the one function whose switch generates it landed.

**This is done, for the LAST run rather than the first.** func_800595C0
matched (see the comment on it in `csrc/unk_1C540.c`) and jtbl_8003310C is
placed at 0x8003310C by a segment of its own; `make check` is byte-identical
and `jtbl_8003310C` has dropped out of `undefined_syms_auto`. The run chosen
is the one whose single function was closest, not the first in the file -- and
because it is the unit's LAST run, it is also the one that keeps working for
the longest: the other three tables are emitted ahead of it in the same
`.rodata`, so landing func_80054F9C, func_80057A94 or func_80057D20 breaks the
placement and forces the split below.

Two traps met doing it. `splat split` with no `--modes` REWRITES
`slus_006.14_undefined_syms_auto.txt` and `..._undefined_funcs_auto.txt` from
only what this one yaml disassembles, which drops every symbol the psyq asm
needs and leaves the link with hundreds of undefined references -- the
Makefile's `build/%.ld` rule runs exactly that, so after a yaml edit both
files have to be restored from git. And the new `bin` subsegment's asset has
to be extracted (`splat split ... --modes ld bin`) before the link, or ld
fails on a missing `build/assets/<off>.o`.

The **second** table does need `unk_1C540` split into separate translation
units, one per run, with the boundaries at the function addresses the run order
implies. Codegen per function does not depend on which other functions share
the file, but the file-scope `__asm__(".sdata\n\t.size ...")` size records and
the `extern u32 D_800814A0a[] __asm__("D_800814A0")` alias at the top of
`csrc/unk_1C540.c` are file-scope and must be copied into every piece.

**The const arrays immediately before each table are not an obstacle.**
D_8002E5D8/D_8002E5E8 (before jtbl_8002E5F8), D_8002E950 (before jtbl_8002E964)
and D_80032E0C (before jtbl_80032E14) are small const data tables, i.e. the
original units' other `.rodata` -- but they can stay in the `bin` blob, because
a `.rodata` subsegment only has to equal what the sibling C object actually
emits. They would only have to be reconstructed as `static const` in the C if
one object had to supply two runs, which the split avoids.

## func_80042560  (unk_14934) -- SOLVED 2026-10-22

Landed in `csrc/unk_14934.c`, 56 of 56 words, with the load-bearing shapes
commented there.

Kept because of how it was wrong. This section argued that loop 1's order --
`sb 0($s0)` / `sb 1($s0)` / `lb 0($s0)`, where sched1 instead fills the load
delay with the independent second `sb` -- required a dependence between two
one-byte accesses at disjoint constant offsets off one base pseudo, that
`memrefs_conflict_p` can always prove those disjoint, and that
`true_dependence`'s differing-flags exception only ever removes dependences.
All of that is correct. The conclusion drawn from it was not, because **those
four instructions were never four insns**: a small byte-aligned struct
assignment stays a single `movstrsi_internal` through combine, sched1,
global-alloc and sched2, and the `lb`/`lb`/`sb`/`sb` appear only from its output
template at final. There was nothing to make dependent.

The same insn is what fixed the three `$s` registers, by removing one in-loop
reference from each address pseudo and taking the walker giv across the
`floor_log2` step at 8 references -- the reference-count lever running downward.

**The failure mode to carry forward: before concluding that a pass cannot be
steered, check that the instructions in question are separate insns at all.**
Everything ruled out here was ruled out against the field-by-field spelling,
which was the wrong question.

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

## func_800423C0  (unk_14934) -- SOLVED 2026-10-22

Landed in `csrc/unk_14934.c`, 72 of 72 words, with all five load-bearing
shapes commented there.

Kept here because the last step generalises. It had reached "every instruction,
immediate and displacement matches, 12 register fields wrong": `r` wanted `$a0`
and the base pointer `$v1` and they came out the other way round, on an
allocation-priority gap of 7% -- `floor_log2(4) * 4 / 8 = 1.00` against
`floor_log2(7) * 7 / 15 = 0.93` -- with whichever was allocated first taking
`$v1`. The note here said the three numbers behind it all looked fixed by the
target's own code, and they were. What was missed is that a reference count can
be raised *without* changing the emitted code, because **cross-jumping runs in
jump2, after global-alloc**: duplicating one byte store into both arms of an
inner `if` took the pointer to 8 references across 18 insns, crossed the
`floor_log2` boundary to `3 * 8 / 18 = 1.33`, flipped the allocation order from
`95 113 ...` to `113 95 ...`, and jump2 then merged the copies back for an
unchanged 72 words. Duplicating a different store in the same block worked
identically.

So "the priority gap is small but the numbers are fixed" is not the dead end it
reads as. Check it against the remaining allocation cases: it needs a pseudo
whose reference count is just under a power of two, and an `if` in its live
range to duplicate into. It does **not** rescue func_80047054 or func_800407C0,
whose gaps are five-fold and which are blocked on a shared `$a0` preference
rather than on priority. Now in `docs/matching-levers.md` section 3.

## func_80041E70  (unk_14934) -- closed with a proof, still 4 words short

Kept at 270 of 270 words and 4 DIFF, and the reason is now a chain of measured
facts rather than "cse prefers the constant".

`rtx_cost` explains why the constant *wins* once cse knows it. What decides
whether cse knows it is the **run boundary**: a cse run is a maximal linear
insn range that ends only at a `CODE_LABEL` with `LABEL_NUSES != 1`. It
follows a conditional jump's fall-through *and* its target, and crosses
barriers and single-use labels. Four probes: body as fall-through, body as a
`bne` target, body behind an unconditional jump plus a barrier -- all fold; a
body label with **two** uses -- the `sll` survives. No `-fno-cse-*` flag
changes any of it.

Three things then block buying that two-use label, each a useful fact on its
own: `thread_jumps` runs inside jump1 before cse, so a provably redundant
second predecessor is already gone (three equivalent OR-ed tests compile to one
`beqz` with the body as fall-through, still `li 32`); jump1 also does
dead-pseudo-set DCE, so a dead assignment cannot hold an arm open; and cse
deletes a redundant store of the same value to the same address, so a
duplicated store cannot either.

And the target's first block provably contains no label: its prologue is
interleaved with a store, and a constant materialisation fills the load-delay
slot between a load and its consumer. Only a scheduler does either, and
schedulers do not cross labels. So the only available boundary is the body's
own entry label, which is the case that folds.

The one zero-word way to make a run boundary -- an `if`/`else` with identical
arms, now in matching-levers section 1 -- is also a scheduler boundary, and
using it here gives 270 words with the body byte-exact and two words lost in
the first block instead. A shape with an opaque third test reproduces the body
byte-for-byte at +2 words, which is the proof that the rest of the function is
right.

Also corrected: the near-miss header's hypothesis that the three
sign-extend/register words are **downstream** of the fold. They are not --
with the fold defeated and the body byte-exact, the call result still comes out
`sll`/`sra` coalesced onto `$v0`. That is an independent allocation problem and
wants its own small dispatch.

## The real remaining blocker: splitting csrc/unk_14934.c

Two of the three big items on this list have turned out to need the same
structural change, and it is worth stating once rather than twice.

**The jump-table group** needs `csrc/unk_14934.c` split at func_80041588,
because the pad word at 0x8002D654 proves func_80041344's table ends one
original translation unit and func_80041588's begins the next, and a single
unit would emit all three tables back to back with no pad.

**The addressing group** needs it too, for a different reason. The mechanism is
solved (a unit that *defines* a small object gets `%gp_rel` for it while
undefined symbols stay absolute -- see
`docs/addressing-and-translation-units.md`), and eleven functions are already
verified against it in `docs/near-misses/gp-window-batch.c`. But the definition
must sit in the same object file as the code, so each unit has to define the
objects its own functions reach. The objects do cluster by unit, exactly as the
addressing analysis predicts, which is further evidence for the split. The
obstacles are that a unit's `.sdata` is one contiguous block while the objects'
addresses are scattered through the window, and that the two functions which
would otherwise be a cheap `-G8` carve-out -- func_80044618 and func_80044698,
consecutive and with no absolute references -- sit in the **middle** of
`csrc/unk_14934.c`, so carving them means splitting it in three and renaming
the tail.

So the next real decision is not "where does the project's data live" but
**where are unk_14934's original translation-unit boundaries, and split it
there**. The jump tables give one boundary (func_80041588) and the
`$gp`-relative clustering gives independent evidence for others. Doing it once
unblocks about 1550 instructions of switch functions and most of the addressing
group together.

## Functions blocked by the translation-unit problem

32 of the remaining stubs cannot match in their current unit, because the
addressing mode they need is per-compilation -- see
`docs/addressing-and-translation-units.md`. Five reach their globals only
`$gp`-relatively (func_8003E4FC, func_8003F5E0, func_80044618, func_80044698,
func_800448BC) and 27 do both and so cannot match under either `-G`.

Two of the five are cheap: `func_8003F5E0` is the first function of
`unk_12DEC` and sits immediately after the carved-out `gprel_12DD4`, so
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

## func_8003FE78  (unk_12DEC) -- resolved

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
