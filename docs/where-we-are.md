# Where the decompilation is, and what to do next

Written 2026-10-23. Read this first, then `docs/compiler-version.md`.

## State

**`guard.py` reads 52 asm stubs as of 2026-10-28** -- 177 known, 177 accounted
for -- and the build is byte-identical; `make check` is green at HEAD.
`progress.py` puts it at 35371 of 41767 words, 84.7%, with 51 of the 52 stubs
parked. NOTE that guard.py under-reported until 2026-10-28: its definition
pattern wanted `) {` and so missed an OLD-STYLE (K&R) parameter list, which
this project writes deliberately to narrow a parameter without putting a narrow
prototype in scope. It was reporting the landed func_8005C2C0 as MISSING. The paragraph below is the
2026-10-23 picture and is kept for its scope caveat, not its count.

**WHERE 2026-10-28 LEFT IT, so the next session does not re-derive it.** Three
functions landed -- func_80059DAC, and the byte-identical pair func_8003A060 /
func_8003A858, which had been the oldest 2-DIFF park -- and three more improved:
func_8005C4D0 22 -> 15 DIFF, func_8005B4D0 379 -> 377 words and 367 -> 344,
func_8005BBFC 220 -> 209 with its whole 26-word prologue exact. Every one of
those came from a MECHANICAL sweep or from reading the compiler, and none from
another source-shape guess.

**AND THE A060 LANDING RETIRES A STANDING "UNREACHABLE".** That park had been
argued closed three separate times -- on `rank_for_schedule`'s class key, on
priority's insensitivity to ALU chains, and on `schedule_select` -- and each
argument was about the ORDER of a fixed set of insns. What actually moved it was
reading sched1's own ready lists instead of reasoning about them: every insn in
the contested group prints `7f000001`, so the only live key was LUID, and LUID
is decided by WHICH STATEMENT generates each insn. Splitting one term of an
expression into its own new local moved the boundary. The transferable lesson is
in matching-levers section 8 under "when both competing insns are bumped"; the
procedural one is that a park's conclusion should name the key the DUMP shows
deciding, not the key the source reads as if it would.

* **ONE NEW TOOL, AND IT NEEDS READING WITH ITS RECORD.**
  `tools/decomp/unname.py` deletes each of a park's locals in turn and
  re-scores; whether a value has a NAME is a compiler switch, and two of
  today's three real improvements came out of it. SWEEPS COMPOSE -- re-run it
  after adopting a hit, which is how func_8005C4D0 went 22 -> 18 -> 15.
  `search.py` gained `t_split_local` (the reverse transform, sound, no hits
  yet) and `t_copy_param` (three hits, ALL THREE rejected -- read its docstring
  before trusting one). `tools/decomp/nmcheck.py`, which already checked every
  park's headline against a fresh build, now also warns where a park's
  signature disagrees with its csrc prototype; all 54 headlines are honest.
* **AND A SOUNDNESS BUG WAS FOUND IN AN OLD FAMILY, so re-check anything a
  sweep told you before today.** `search.py`'s `t_hoist_read` matched store
  DESTINATIONS as well as reads and substituted its fresh local there, which
  deletes the store and scores a word better for it. On func_8005B4D0 that was
  the best-looking result of the day -- 376 words exact, 333 DIFF against 344 --
  and it had dropped `*(u32 *) (v + 0x64) = t;`. Twelve of twenty `_READ`
  matches in that file are lvalues, and fixing it took a full pass from 562
  candidates to 350 WITH a family added. Scope, so nobody over-reacts: negative
  sweep results are unaffected, since nothing improved either way; only a pass
  that ACCEPTED a hoist and continued greedily is void, and func_8005B4D0 was
  the only one. All families are now audited and classified in search.py's
  header as pure rewrites or semantic levers.
* **A VARIANT THAT CHANGES THE WORD COUNT MUST BE CHECKED AGAINST THE
  TARGET'S INSTRUCTIONS, not just its score.** This is the specific form of
  "read the hit", and the vague form did not stop me adopting a wrong source
  on 2026-10-28: func_8005A90C went from 100 words (three short) to 103
  (exact) at 59 DIFF against 92, and I committed it. The three words were a
  FOURTH parameter copy into a fifth callee-saved register -- and the
  listing's prologue has exactly THREE parameter copies and six callee-saved
  registers, so the count went exact by adding an instruction the target does
  not contain. Reverted. The check that would have caught it is one diff:
  name the instructions the variant adds or removes, then find them in the
  listing. An exact word count reached by a spurious instruction is worse than
  being three short, because the park then tells the next attempt the shape is
  right.
* **AND WHEN A CHANGE REWRITES AN EXPRESSION THE LISTING SHOWS, TRACE THAT
  INSTRUCTION BEFORE CHANGING IT.** This is the OTHER retraction of
  2026-10-28, and the word-count rule above does not catch it, because the
  word count did not move. func_8005BBFC went 220 -> 219 by writing its range
  test on the RAW parameter instead of `(s16) vel`, which buys `vel` a third
  reference and keeps its load in the prologue. The listing settles it in four
  instructions:

      sll $v0, $t0, 16 ; sra $a2, $v0, 16 ; lbu $v0, 0x6($s1) ;
      slt $v0, $a2, $v0

  the test reads `$a2`, which is the CAST. Reverted to 220. A test, a cast, a
  comparison's operand order and a shift width are all VISIBLE in the listing,
  so a change to one is checkable in a minute -- and a reference-count argument
  for changing it is not evidence about the original. The same trace also
  retired that park's central theory: both its sunk parameters have exactly two
  references in the listing too, so reference counts were never what kept their
  loads in the prologue.
* **THE NUMBER-AGAINST-SHAPE CALL IS NOW CONSISTENT ACROSS THE PARKS**, audited
  2026-10-28. Five files weigh a better score against a shape the listing
  shows: func_800563B0 (keeps 14, records 11), func_8005914C (keeps 205/131,
  records 203/116), func_8005BBFC (keeps 220, records 219), func_8005A778
  (keeps the in-place extension at the higher count) and func_8005A608 -- which
  held the `(s16)` cast at 11 DIFF while its own text said the in-place form at
  13 was "structurally closer". The listing has
  `sll $a1, $a1, 16` / `sra $a1, $a1, 16`, same register, so the cast's
  two-register pair appears nowhere in it; that file now holds the 13. The rule
  the others follow, from func_800563B0: **the better NUMBER is not the better
  SOURCE**, because a park's job is to be the best candidate for the ORIGINAL,
  and a source that scores well on an instruction the target lacks tells the
  next attempt its shape is right.
* **AND READ EVERY HIT BEFORE ADOPTING IT.** `unname.py` produced three
  proposals today with BETTER numbers that were different programs or the
  wrong shape: a value whose operand was incremented in between, a duplicated
  function call, and two cases where the listing demonstrably keeps the local
  the sweep deletes. Its guards catch the first two classes now; nothing but
  the listing catches the third.
* **THE `csrc/` PROTOTYPES ARE NOT EVIDENCE ABOUT A *REGISTER* PARAMETER'S
  WIDTH -- BUT THEY ARE THE ONLY WITNESS FOR A *STACK* ONE.** The first half
  was audited on 2026-10-28 and the second half was learned the hard way four
  hours later, so take them together. For an argument in `$a0-$a3` the
  caller's own argument type already fixes the generated code, so a prototype
  declaring it narrow or wide changes nothing and proves nothing: twelve
  parks' assumed signatures disagree with `csrc/` and in every one the
  caller's argument is already the narrower type -- `func_80055D84(s16 k)`
  passing `k` to func_8005A778's second parameter, declared `s16` there and
  `s32` in the park, is the shape of all of them -- so the declared width
  changes no caller code and neither declaration is testimony. What the list IS
  is a landing checklist: the prototype in scope is what every caller compiled
  against, so landing a park whose signature differs rewrites those callers,
  which is the trap func_8003AB44's landing hit. `nmcheck.py` now prints it.

  FOR A STACK-PASSED ARGUMENT IT IS DIFFERENT. The callee loads a full word
  whatever the declared width (the o32 caller widens), so its own listing
  cannot tell you -- and the prototype becomes the only opinion on record.
  func_8005BBFC's prototype declared all eight parameters `s16`; the park had
  `s32 vel, s32 fine`, dismissed `s16` as noise on a one-word score, and
  reasoned against it from the `lw`. Adopting `s16` took it from 220 to 209
  DIFF with its whole prologue exact. So when a mismatch concerns a parameter
  beyond the fourth, test it.
* **THE SUB-12-DIFF PARKS ARE NOW EACH BOUNDED ON A NAMED COMPILER FACT**
  rather than on "an allocation question" -- see each file's last dated entry.
  Several standing "first thing to try" items were tried and closed today
  (func_8003A374's live-range term,
  func_8004F330's `$a0` preference, func_8005ACDC's hoist, func_80048C3C's
  combine fold, func_80038CB8's volatile route, and the second-set escape in
  five files). So the cheap end is genuinely worked out, and the section
  "Audit the context before sweeping the source" below is now the live advice:
  what moves next is a changed assumption -- a field width, a parameter type, a
  unit boundary -- not another spelling.

**THERE IS EXACTLY ONE DUPLICATED ROUTINE in the resident executable, and no
stub duplicates already-landed code.** Checked 2026-10-28 by taking each of the
211 stub listings' instruction bytes and searching the whole 0x80000 image for
them: every stub occurs exactly once except func_8003A060 and func_8003A858,
which are the same 43 words at 0xD860 and 0xE058 -- they shared a carved-sweep
row and landed together on 2026-10-28. So the tempting idea -- that some stub is a copy of a
function already in C and could be landed by transferring that source -- is
closed. It cost two minutes; do not re-speculate it. (The check works because
MIPS branches are PC-relative while `jal` targets and `lui`/`addiu` symbol
halves are absolute and identical in a true copy, so a duplicated routine has
byte-identical text wherever it sits.)

**849 of 1008 functions in the RESIDENT EXECUTABLE are C and the build is
byte-identical.** `make check` is green at HEAD. The remaining 159 are
`INCLUDE_ASM` stubs in `csrc/`: 154 in the four units carved on 2026-10-23 and
5 long-standing near-misses. **That count is the RESIDENT EXECUTABLE only.**
`csrc/` covers no other file and `guard.py` counts nothing else, so quote it as
"164 of the resident executable's 1008" -- the scope section below is why the
number looks small next to the game. `python3 tools/decomp/guard.py` is the authority
on that count; this file has been wrong about it before, so quote it and not
this paragraph.

For the per-unit picture -- how many of each file's functions are C, how many
are still stubs, how many of those are parked, and what share of the unit's
WORDS is done -- run `python3 tools/decomp/progress.py`. It takes unit names
to list one file's remaining stubs smallest-first with each one's park state,
and `--parked` to rank every park file by DIFF. The word columns read sizes
out of `build/csrc/*.o`, so they want a build and are as stale as `build/`;
the function counts do not.

**Pick the next function with `python3 tools/decomp/triage.py --easy --fresh`,
not by word count.** Word count is a poor proxy for effort: measured over
2026-10-26, call sequences and struct setups landed on the first or second
attempt at any size, while every function parked at the exact word count was
either a loop whose address arithmetic loop.c reduced differently or a register
permutation in a branch-heavy body. triage.py scores the SHAPE of the target
listing instead -- loops, jump tables, multiplies and stack switches are the
expensive features, calls and stores are the cheap ones -- and `--fresh` drops
what carved-sweep.md already has a measurement for.

**The carved-unit sweep is the live work**, and `docs/carved-sweep.md` is its
record: the source patterns that keep paying, the declaration traps, and a
parked list with measurements. On 2026-10-23 it went from 645 stubs to 237, in
four sessions; on 2026-10-24 from 237 to 186.

**Both of the structural blockers that shaped most of this project are gone**,
and neither needed the thing it was being built towards:

* **Jump-table placement.** Each table is in `.rdata` under `.align 3`, and the
  leading `.align 3` is a no-op at section offset 0 -- which explains the whole
  region and means no C file has to be split. Placement is a `.rodata`
  subsegment per unit (with the leading dot), one splat segment per run, and
  `align: 4`. See the jump-table section of `docs/open-matching-problems.md`.
* **One object reached both `$gp`-relatively and absolutely.** A bare
  `__asm__(".sdata\n\t.size SYM,N\n\t.text");` puts the symbol in maspsx's
  table without defining anything, so maspsx rewrites loads and stores to
  `%gp_rel` while leaving address materialisation absolute -- and there is no
  linker placement to arrange, because the symbol stays undefined. See
  `docs/addressing-and-translation-units.md`. **The window definition that
  document spends most of its length deriving is no longer needed.**

### Scope: this is the engine, not the game

**Read this before quoting the number above.** "191 functions" is every
function in `data/slus_006.14`'s own code, which is 43 KB of a 512 KB
executable. It is *not* the game. Most of Azure Dreams' code lives in the
overlays, and essentially none of it is analysed:

| file | size | code-bearing (4 KB blocks, all distinct) |
|------|------|------------------------------------------|
| `slus_006.14` | 0.5 MB | ~216 KB -- 43 KB of game code plus PSY-Q library and crt0 |
| `main.bin` | 2.5 MB | ~80 KB |
| `town.bin` | 16.1 MB | ~1.0 MB |
| `dungeon.bin` | 26.5 MB | ~7.3 MB |

Measured by counting 4 KB blocks holding at least two `addiu $sp,$sp,-N`
prologues *and* two `jr $ra`, then deduplicating by block hash: nothing
repeats, so the volume is real. A sample from `dungeon.bin` at file offset
0xA6000 disassembles to obvious compiler output -- the same
`sll 16`/`sra 16`/`slti` clamp idiom, argument-register moves and filled delay
slots we match here. Treat the figures as order-of-magnitude: the heuristic can
count packed data and can miss sparse code, and `dungeon.bin` is certainly an
archive of many separately loaded entries rather than one image.

So the overlays hold roughly 8.4 MB of code against the executable's 216 KB.
`make check` verifies **only** `slus_006.14`; `dungeon.bin` has no splat spec at
all, `town.bin` has one `bin` blob covering all 16 MB, and `main.bin`'s 28
subsegments are all `bin`.

### And most of slus_006.14 itself is not accounted for either

The scope caveat above is about the overlays. There is a second one inside the
resident executable, and it is easy to miss because splat's summary line
("Split 98 KB (18.72%) in defined segments") does not break the remainder down.
Measured in 4 KB chunks over every `bin` subsegment, classifying each by zero
density, Shift-JIS density, MIPS prologue/`jr $ra` count and Shannon entropy:

| what | bytes | % of the 512 KB |
|------|-------|-----------------|
| high-entropy data (entropy 7.98, almost certainly packed assets) | 175,552 | 33.5% |
| **code -- PSY-Q library, unidentified** | **123,484** | **23.6%** |
| structured data | 95,180 | 18.2% |
| C translation units (incl. the 5 remaining stubs inside them) | 44,112 | 8.4% |
| identified library/asm (`psyq/*`, `main`) | 51,852 | 9.9% |
| Shift-JIS text | 30,144 | 5.7% |
| header, data, .rodata, split strings | ~4,000 | 0.7% |

**So about 120 KB of this file is code nobody has identified -- roughly three
times the volume of the game code that is decompiled.** It is not game code: it
sits in the library regions, mostly 0x0072A8-0x0101F8 (36 KB, 348 prologues,
immediately *before* `main`, so crt0 plus linked library) and
0x01C540-0x0301A8 (81 KB, 625 prologues, interleaved with the `libsnd` and
`libspu` modules splat already places by content). The 51 KB already attributed
as `asm, psyq/*` is where that identification succeeded.

**And it is not the mechanical gap it looks like.** Investigated 2026-10-23.
`azure libmatch psyq` confirms the library set is the right release -- 162 of
163 already-placed modules match exactly, mean fuzzy 0.998 -- yet
`azure libscan psyq` over both large code blocks (0x72a8-0x101f8 and
0x1c540-0x301a8) locates **zero** of 1257 modules examined, at `--min-invariant`
4 and 12 alike. The only hits are tiny stubs matching ambiguously at many sites
on 4-8 invariant words. So this code is not any module of the twenty libraries
in `psyq/lib`, although those are provably the ones the game linked against.

### The code accounting is complete: 1008 functions, 186 of them C

**Settled 2026-10-23, and this is the authoritative count.** It was revised
upward four times in one afternoon as blobs were carved -- 191, then ~421 from a
`jal` estimate, then 784, then 1008 -- so the number is stated here once, with
the survey that makes it final.

| unit | file range | functions |
|------|-----------|-----------|
| previously typed | various | 191 (186 C, 5 stubs) |
| `unk_72A8` | 0x072a8-0x101f8 | 294 |
| `unk_1C540` | 0x1c540-0x301a8 | 465 |
| `unk_1AF38` | 0x1af38-0x1c520 | 53 |
| `unk_301D4` | 0x301d4-0x30290 | 5 |
| **total** | | **1008** |

`guard.py` is the authority and reads 406 stubs as of the end of
2026-10-23's sweep; `make check` is byte-identical at e6bfbb9 after every
carve.

**Why this is final rather than another revision.** Every remaining `bin` range
was surveyed for `addiu $sp` prologues, `jr $ra` returns, and how many `jal`
instructions target it from outside. Exactly one was code-bearing (188 bytes),
and it is now carved. The rest is data and matches the entropy profiles already
recorded: 25 KB of Shift-JIS text at 0x800 and 0xefc, ~5.5 KB of data between
0x31cc4 and 0x363a8, 90 KB of structured data at 0x3e01c, and 180 KB of
packed assets at 0x54240 (entropy 7.98). The 80 `jal` targets landing in that
last region are the engine calling **out-of-image overlay addresses**, which
`docs/overlays.md` covers.

So every code-bearing byte of `slus_006.14` now sits in a typed subsegment, and
what is left as "unknown" is data.

### These 817 functions are the game's own code

Resolved 2026-10-23, and it reframes the project. Both blocks are now carved as
C subsegments (`unk_72A8` and `unk_1C540`), `make check` is byte-identical, and
splat's own disassembly counts them:

| block | file range | vram | functions |
|-------|-----------|------|-----------|
| A | 0x072a8-0x101f8 | 0x80033aa8-0x8003c9f8 | **294** |
| B | 0x1c540-0x301a8 | 0x80048d40-0x8005c9a8 | **465** |

So 759 functions, plus the 191 previously typed, is **784** -- the headline
figure was a quarter of the executable's real function count. An earlier `jal`
scan estimated 421; it counted only direct-call TARGETS and so missed everything
reached indirectly.

**They are the game's own code, not SDK modules.** The decisive evidence is
what they CALL: block A calls `AddPrim`, `GetTPage`, `ClearImage`,
`RotTransPers4`, `RotMatrix`, `CompMatrix`, `PushMatrix`/`PopMatrix`,
`ScaleMatrix`, `LoadImage`, `VSync`; block B calls `DrawSync`, `SetSemiTrans`,
`SetRotMatrix`/`SetTransMatrix`, plus `bzero`, `memcpy`, `strcpy`, `strncpy`,
`strrchr` and `EnterCriticalSection`/`ExitCriticalSection`. That is libgpu,
libgte, libc and libapi being *consumed*, and it is the same vocabulary as the
engine code already decompiled -- func_800453E0 and func_80045CC4 call
`RotTransPers4` and `RotAverage4` and link primitives with the `addPrim`
bitfield store. Supporting evidence, gathered before the carve:
Both blocks are cohesive -- the overwhelming majority of calls into them come
from within themselves -- and both are called by the game code we have already
decompiled.

**Three independent reasons they are not library modules we could just place:**

1. `azure libmatch psyq` says the library set is right (162/163 exact, mean
   fuzzy 0.998), so a miss is not a wrong-release artifact.
2. `azure libscan psyq` locates **zero** of 1257 modules in either block.
3. Byte-exact search: the first function at 0x80033aa8 is a bitmap set/clear/
   test triple (reject id 0 and 1, `id >> 5` for the word, `id & 31` for the
   bit, table at 0x80012D70 -- one of the out-of-image constant addresses this
   project already documents as the game's own idiom). Its relocation-free
   instruction run appears **twice in the executable and in none of the twenty
   `.lib` files**, and content matching demonstrably works here, since that is
   how libmatch placed 162 modules.

Block A begins immediately after `psyq/libsn/SNMAIN` ends -- `__SN_ENTRY_POINT`,
`__main` and `__do_global_dtors` are the last symbols before it -- so it sits
exactly where a PSY-Q link puts the first linked objects, which is consistent
with it being the game's own earliest objects rather than libraries.

**What is not settled:** whether this is the game's own code or a third-party /
engine library statically linked from something not in `psyq/lib`. Either way it
is 117 KB of unmapped, game-reachable code in the file `make check` verifies,
and the function count in this document was badly wrong. The machinery is all here and working --
`azure lib --list`, `azure libmatch`, `azure libscan`, and `tools/lib2ar.sh`
for `nm`/`objdump` introspection; note `libmatch` and `libscan` read `.lib`
natively and do **not** need pcsx-redux's `psyq-obj-parser`, which only
`lib2ar.sh` uses.

What the resident engine being nearly finished *does* mean: overlay code calls
into it, and the 23 called-but-undefined functions above 0x8008875F are the
engine calling out, so the engine genuinely had to come first. But the honest
framing is "the engine is nearly done, the game content is unmapped", not "the
game is 85% decompiled". See `docs/overlays.md`.

**And "191 functions" is NOT this executable's function count.** That claim was
in this document until 2026-10-23 and it is wrong by about a factor of three --
see the section below. 186 of 191 counts the functions splat has *typed as C*,
which is 8.4% of the file's bytes.

Run `make check` before trusting anything. Do **not** pipe it through `tail` or
`head`: that masks make's exit code, and a broken state was committed that way
once. Use

    make check > /tmp/check.log 2>&1; echo rc=$?

`asm/` is generated and gitignored, and splat only *adds* `nonmatchings`
listings, so the reference listing for a function you have already decompiled
stays until the next `make splat` (which does `rm -rf asm/nonmatchings` first).
Do not run `make splat` while anything else is reading those listings.

Before writing a function, read **`docs/matching-levers.md`**. It is the
project's record of how this compiler behaves: the pass order and what each
pass decides, the measured source shapes that move it with the word cost of
reverting each, the dead ends not to retry, and the toolchain traps. It exists
so nothing gets re-derived, so **add to it whenever a session learns
something** -- that has been the single biggest waste of effort here.

## The remaining stubs

| group | count | what it means |
|-------|-------|---------------|
| parked, with a COMPILING CANDIDATE | 55 | a park file each in `docs/near-misses/` |
| structurally blocked | 0 | was func_80033D44 + func_80033D54: on 2026-10-27 the old unk_72A8 was CUT in two (`gprel_72A8` + `unk_72A8`) so the pair gets its `%gp_rel(D_8008099C)` while the two landed functions below the cut keep the absolute form. func_80033D44 landed the same hour; func_80033D54 is now counted under never attempted |
| deliberately asm | 1 | func_8004CAA0, 18 words, STACK-SWITCH |
| never attempted | 0 | **all four giants were drafted on 2026-10-27**: func_80033D54 **MATCHED and landed 2026-10-27** (it was at 537 of 537 / 46 DIFF, every instruction right, 34 of them four callee-saved register names; the phantom bit test in matching-levers section 3 carried the two missing references and is marked SYMPTOM in the landed source), func_800345B8 at 546 of 546 / 128 DIFF (two adjacent allocno pairs), func_8004C36C at 464 of 461 / 171 DIFF (three load-delay nops and what follows them) and func_80056E10 at 713 of 718 / 687 DIFF (first reading; the seven scans are the new part) |

## Re-auditing a parked conclusion

Several park conclusions this project has leaned on turned out to be stale or
to rest on a measurement that could not have tested its own hypothesis. That is
now a thing to check deliberately, not a thing to discover by accident, and
four of the checks are mechanical. A one-off script for them lives outside the
repo; what matters is the checks.

1. **HEADLINE.** Compare the park's own `N of M words, D DIFF` first line with
   `verify.py` today. Swept over every park 2026-10-27: all of them agree,
   which is a real credit to the discipline here -- so when a park is stale it
   is stale in the BODY, not the headline. func_80056DB4's "WHAT IS LEFT"
   described a `lw`/`li` pair that had already come right while the headline's
   2 DIFF was correct.
2. **VERDICT vs the header's own category.** Run `advise.py` and compare its
   SHAPE/ALLOC/LENGTH verdict with what the header reasons about. TWO FALSE
   POSITIVE MODES, both found immediately: advise reports the callee-saved
   `sw`/`lw` stack-slot offsets as an IMMEDIATE finding when the register
   assignment differs (func_8003AB44 -- the park is right to call those a
   consequence), and a header that names a direction in order to RULE IT OUT
   ("the statement order is pinned") reads as if it were reasoning that way
   (func_8003A374, func_8003A3E8). Treat this check as a prompt to read, not a
   finding.
3. **ALLOCATOR MISMATCH.** If the header invokes local-alloc machinery --
   `qty_compare`, `qty_phys_*`, a copy suggestion -- check whether the pseudos
   it cites are `global` in `--greg`'s table, and vice versa. A hypothesis
   about local-alloc tested with a function-scope variable has not been tested
   at all, because that variable spans blocks and global-alloc sees it. That is
   exactly what happened to func_8003F368's four 2026-10-23 measurements.
4. **DOUBLED LIVE LENGTH.** Flag any `--greg` row whose live length exceeds
   about 1.5x the function's word count. `update_equiv_regs` doubles
   `reg_live_length` for a once-set pseudo with a constant REG_EQUAL note (see
   matching-levers section 3), and global-alloc reads the doubled value, so
   those rows' priorities are half what the real live ranges give and the
   MARGINS table's "live <= N" instructions for them are unreachable. The tell
   is an impossible number: func_80047054 is a 58-word function whose pseudos
   80-83 all report live 88, and its margin row asks for "live <= 17 (-71)".

5. **"IT IS AN ALLOCATION QUESTION" IS THE MOST OVERTURNED CLAIM IN THESE
   FILES.** On 2026-10-28 five park conclusions of that form were wrong, and
   all five failed the same way -- the park read a register permutation off the
   diff, went to `--greg`, found a priority order that explained it, and
   stopped. The priorities were real; they were not the cause. In order of how
   much time each cost:

   * func_8003AB44, "a pure 3-cycle register rotation" pointing at
     global-alloc's order: argument 3 was `s32` with an `(s16)` cast instead of
     `s16`. Declaring it narrow put all three callee-saved registers right at
     once. 9 DIFF to MATCH. **Check sub-word PARAMETER TYPES before priorities**
     -- a narrow parameter separates the raw value from its extension and moves
     an allocation order with no section 3 lever at all.
   * func_800565D8, "the lever is local-alloc's SUGGESTION phase": no quantity
     in that block had a suggestion. `combine_regs` had merged the byte with
     its shift, summing their references AND copying up the wider mode's size,
     so the park's "the subtraction already outranks the byte two to one" was
     arithmetic on a quantity that did not exist. 3 DIFF to MATCH.
   * func_80038CB8, "one allocation decision": the register follows the
     SCHEDULE. The listing's `sh` to +0x20 frees $v0 before the +0x28 byte is
     loaded, so the temp reuses it; ours sinks the store and the temp takes
     $v1, pushing the carried pointer off $v1.
   * func_8004D880, "whatever bars the parameter from $a0 is the whole
     finding": nothing can, in that shape. `find_reg` overrides its
     lowest-legal pick with the allocno's own copy preference, and
     `regs_someone_prefers` is consulted only in pass 0, which never allocates
     a register for the first time. The listing uses a THIRD pseudo. 22 to 12.
   * func_8004D294, "no spelling of the subtraction moves it, the next step is
     sched.py": a statement split moved it. 9 DIFF to MATCH.

   So the order to work an ALLOC verdict in is: parameter and field TYPES
   first, then whether the rival quantities are MERGED (`advise.py --greg` now
   warns about this), then whether a store or a load's POSITION decides the
   register, and only then the priority table.

6. **WHETHER A VALUE HAS A NAME IS A SWITCH, and it is cheap to try.** A
   declared local's pseudo carries `REG_USERVAR_P`; one that expand or cse
   invented does not, and the dumps print it as `/v`. On this target that flag
   decides whether a giv pays `copy_cost` -- i.e. whether an address is
   strength-reduced at all -- and whether an invariant can be hoisted to the
   preheader. func_80059DAC landed on 2026-10-28 purely by deleting a
   `u32 *d` local and writing its address out at each use: 8 DIFF to MATCH,
   same statements, same order. So when a diff is "the target walks a pointer
   and we recompute" or the reverse, try both spellings BEFORE theorising; and
   read `matching-levers`' own `REG_USERVAR_P` section first, because most of
   the places the flag is mentioned in the compiler are unreachable here --
   combine in particular cannot be defeated this way.

A MEASURED NEGATIVE worth not re-deriving, 2026-10-27: converting a park's
definition to an OLD-STYLE (K&R) parameter list, with its declared types
unchanged, was swept over all 38 multi-parameter park files and changed not one
word in any of them. The declaration style reaches nothing; what moved
func_8005C2C0 was NARROWING three parameters to sub-word types, and the
difference between those two statements is the whole lever (section 8 of
docs/matching-levers.md). The sweep also means the sub-word lever is only worth
trying where the listing's own evidence says a parameter is narrow -- a
truncation, an `andi 0xFFFF`, a `sll`/`sra` pair on an incoming register -- not
as a blind spelling.

**EVERY NEAR-MISS NOW HAS A COMPILING CANDIDATE**, as of the end of
2026-10-27. That was not true for most of the day: 44 of the 70 remaining
stubs were recorded only as prose rows in `docs/carved-sweep.md`, several of
them COMBINED rows covering two or three functions at once, and a row has
measurements but no source -- so nothing could be swept against it and the
next reader had to reconstruct the candidate from the prose before doing any
work. Reconstructing them is most of the work of landing one: of the rows
turned into parks that day, EIGHT MATCHED outright (func_800589B8,
func_80033AE8, func_800353F4, func_80058510, func_8003B714, func_80050A58,
func_8003A2E0 and func_8003A10C) and six more came out
measurably better than the row claimed -- func_80049E1C 18 DIFF to 8,
func_80055864 19 to 3, func_8004E9EC's row could not be reproduced at all
(its shape measures 34 words against the 33 it claimed), func_8003A3E8 and
func_8003A374 15 to 12, and func_80056DB4 4 to 2.

So the first move on a near-miss is now `python3 tools/decomp/progress.py
--parked` and then the park file, and the rows are there for their negatives
rather than as the record of record.

**THE "ONLY TWO NEVER ATTEMPTED" CLAIM THAT STOOD HERE WAS WRONG**, and the
audit that caught it is worth re-running: a stub counts as attempted only if
it has a park file in `docs/near-misses/` or a ROW OF ITS OWN in
`docs/carved-sweep.md`, and nine did not. Being mentioned inside another
function's row is not a measurement. The list, smallest first:

  func_8004C36C 461 w (GTE model draw, 20 blocks, three stack objects)
  func_800345B8 546 w (GTE sprite transform, the scratchpad as a struct)
  func_80056E10 718 w (the voice family's third member, 47 blocks)

All three now have a SURVEY ROW in `docs/carved-sweep.md` -- frame layout,
stack object bases, the call sequence, the tables each one walks, and for each
the thing to get right first (func_8004C36C's three stack records at +0x18,
+0x94 and +0xFC; func_800345B8's scratchpad struct at 0x1F800000, which is the
only one in the project; func_80056E10's two spilled `u8` parameters and its
three parallel tables). None has a candidate source yet.

**SEVEN OF THE TEN WENT THE SAME DAY THE LIST WAS WRITTEN**, and that is the
argument for working through the rest before grinding the parks -- the parks
have had their transform families swept, these had never been read:

  func_80056948  48 w   MATCHED, first draft
  func_80035484  90 w   MATCHED, first draft
  func_8003C920  54 w   MATCHED, third draft
  func_8005560C  73 w   MATCHED, second draft
  func_80035888  93 w   MATCHED, one statement swap from the first draft
  func_8005ACDC  75 w   parked at 3 DIFF, from 35 on the first draft
  func_80059F8C 145 w   parked at 121 DIFF, structure decoded

Each one took two or three edits from shapes already recorded in
matching-levers.md and in the siblings' headers: the giv lever for
func_8005ACDC, fold's `associate:` for func_8005560C's `<< 1`, the
"N materialisations mean N locals" rule for func_8003C920's two arms, and for
func_80035484 nothing new at all -- its three siblings had settled every
shape. The audit that found the list is five lines of Python over `csrc/` and
the two docs; `guard.py` already checks the converse (a row for a function
that has landed).

Everything else has a park file or a sweep row with measurements behind it, so
for those the next session's first move is
`python3 tools/decomp/progress.py --parked` and then the park, not a fresh
reading of the listing.

**Where the parked work actually is**, from advise.py's verdict over the 44
park files that existed early on 2026-10-27. EIGHTEEN have MATCHED and landed
since that census -- func_8004E6F4, func_80059814, func_800550E8,
func_8004DF8C, func_8004DE88, func_800589B8, func_80033AE8, func_800353F4,
func_80057D20, func_80038F48, func_8003931C, func_8005C2C0, func_80057948,
func_80055D84, func_80033D54, func_8003BD84, func_800537D0 and func_8003666C
-- so read the counts below as the shape of the remaining work,
not as a tally; `progress.py --parked` is the live one.

| verdict | count | what it means for the next attempt |
|---------|-------|------------------------------------|
| SHAPE  | 25 | advise.py names a lever for each; the source is still wrong somewhere |
| LENGTH |  6 | the word count is wrong, which is the most tractable signal there is |
| ALLOC  |  2 | every opcode, immediate and displacement matches; only register numbers differ |

The two ALLOC ones are func_800599B0 (1 register) and func_800407C0 (62)
(func_80055D84, the third, LANDED 2026-10-27 by moving two declarations into
the arms that use them, which moves their pseudos from global-alloc to
local-alloc -- section 3 of matching-levers, and the first thing to try on any
remaining ALLOC row) -- no source rewrite reaches those, so read
matching-levers section 3 and `advise.py --greg` rather than trying
spellings. (func_8003931C, the fourth, LANDED 2026-10-27 on a live-length
trim of exactly two insns -- see matching-levers section 8; func_80038F48
landed the same day on the EXPAND_SUM lever there, and func_8005C2C0 -- 10
words of entry-block SHAPE -- on the SUB-WORD PARAMETER TYPE lever, which is
the one to read first for any diff that wants a `move $sN,$aN` placed later
than the listing has it. That lever was first written up as an old-style
parameter list, which measurement then disproved; the entry in section 8 says
so, because the wrong version is the kind that gets re-derived.) The eleven LENGTH
ones, by how far off they are:

  -1  func_8003800C  func_8004951C  func_8004A330  func_8004B954 (was -7)
  +1  func_8005914C
  +3  func_8005B4D0      -4  func_8005A90C

Everything else is at the exact word count with a named shape finding.

**The lowest-DIFF parks, as of the end of 2026-10-27**, which is where to
start: func_800599B0 (1 DIFF, one register), func_80035E58 (2, one sched2
swap), func_800565D8 (3), func_8004A574 and func_80055D84 (5), func_8003666C
(7). Each of those has a park file whose header says what has been ruled out;
read it before trying a spelling, because most of them have had their obvious
families swept already.

**Four of the day's matches came from rows, not parks**, and that is worth
knowing: a carved-sweep row has measurements but no SOURCE, so nothing can be
swept against it and the next reader has to reconstruct the candidate from the
prose. Reconstructing one took four tries on func_800589B8 and reached MATCH;
func_80033AE8, func_800353F4 and func_8003666C went the same way. If a row is
the only record of a near-miss, writing its source into
`docs/near-misses/` is most of the work of landing it.

Two tools exist for the parks specifically, and both earn their keep:
`search.py` sweeps the source transforms this compiler is sensitive to, and a
run of it over all 40 parks on 2026-10-27 improved NINE of them -- including
func_8004CECC from 101 words (-1) and 101 DIFF to 102 (+0) and 48, and
func_8004E6F4 from 102 (+2) and 101 to 100 (+0) and 17. Every one of those
came from a transform family search.py already had; they had simply never
been swept across the parks.

**EVERY `search.py` SWEEP RECORDED BEFORE 2026-10-28 PREDATES TWO FAMILIES,
and seventeen park files cite one as evidence of exhaustion.** `t_hoist_read`
lifts a memory read into a NEW local and tries each legal POSITION among the
leading initialisers; `t_decl_order_init` transposes adjacent declarations
when either is initialised. Neither existed, and the old set could not express
either shape:

* `t_split_binop` splits a statement IN PLACE and introduces no new local, and
  nothing moved a local's initialiser earlier -- yet that is exactly what
  landed func_8003666C (7 DIFF to MATCH: the +0x22 read hoisted into an `s32`
  local placed BEFORE `n`, where the same local after `n` is still 7).
* `t_decl_order` reads `_decl_run`, which STOPS at the first initialised
  declaration, so on the 7 parks whose leading declarations all carry
  initialisers it had an empty run and generated nothing at all.

So a park header saying a full sweep found nothing is weaker evidence than it
reads, and re-sweeping is cheap and costs no model tokens. The two landings
that exposed this -- func_8003666C and func_8003BD84 -- were both sitting
behind a park's own measured negative that named a spelling without naming its
position. `advise.py` prints a FRAME census whenever the
two builds disagree about a `$sp` slot, which is the sharpest diagnostic
there is on a function that spills.

SEVEN of the executable's eight referenced jump-table runs are placed, and
the one that is not is waiting on func_8004CECC (jtbl_8002E5F8).
jtbl_80032F04 went in on 2026-10-27 with func_80057D20. The first three went in
with the units that own them. The other three all came on 2026-10-27 and
between them settled how this works:

* **A run does not have to be placed whole.** jtbl_80032F04 is the LAST of
  its run's three tables, so the `.rodata` subsegment stops before it and the
  rest stays `bin` -- which landed func_80054F9C and func_80057A94 without
  waiting for func_80057D20.

* **A run can be entered from the middle.** jtbl_8002EFF0 is the SECOND of
  its run's two tables, so its subsegment starts at the second one and the
  five words ahead of it stay raw.

* **One contiguous run can be shared between two units**, through two
  `.rodata` subsegments in two adjacent segments -- which is how
  jtbl_8002EFDC and jtbl_8002EFF0 are supplied once both their functions
  matched.

The price is that a unit emits ONE contiguous `.rodata`, so the carved unit
`unk_1C540` is now cut into FIVE pieces, one run each: `unk_1C540`,
`unk_25944`, `unk_264E0`, `unk_2879C` and `unk_2CDC0`. Two more cuts are
implied when the last two functions land. The mechanics of a cut -- including
the prototype trap that actually bit -- are in the jump-table section of
`docs/open-matching-problems.md`.

### Near-misses, with their files in `docs/near-misses/`

Sorted by how open they look, not by DIFF: a bounded gap at 8 words is further
away than an open one at 47. **BOUNDED** means a numeric bound or a named pass
check has been measured *under this project's current compilation assumptions*
-- not that no C source produces these bytes, which cannot be true. See below.

| function | unit | words | DIFF | state |
|----------|------|-------|------|-------|
| func_8003F368 | unk_12A70 | 113 of 113 | 4 | **BOUNDED**, both gaps named; each costs a word to produce from C |
| func_80041E70 | unk_154BC | 270 of 270 | 1 | **BOUNDED**, one word, owned by cse and argued completely |
| func_8004437C | gprel_17B7C | 100 of 100 | 8 | **BOUNDED**, diagnosed at pseudo level |
| func_800407C0 | unk_12DEC | 178 of 178 | 62 | **BOUNDED**, a real copy preference plus a 5x priority gap |
| func_80047054 | unk_19434 | 58 of 58 | 30 | **BOUNDED**, one common cause, not splittable by any shape yet measured |

### What 191 of 191 would actually take

Worth stating plainly, because the number is easy to assume is close. All five
are near-misses with the source written and checked, and between them they are
1, 4, 8, 30 and 62 differing words. Four of the five have every opcode,
immediate, displacement, branch offset and delay slot matching already and
differ only in which register holds what.

**Do not read BOUNDED as "unreachable".** These bytes were produced by cc1psx
from a C file, so a C source that reproduces them exists -- that is a fact about
how the ROM was built, not a hope. What the headers below have sometimes called
unreachable is always narrower: unreachable *given everything else this project
assumes*. The unit boundary, each symbol's declared type and size, the `-G`
value, which other functions share the translation unit and in what order, the
struct layout, the flags -- every one of those is this project's inference, not
evidence about the original source. "No spelling of `end` moves that
preference" can be a sound measurement and still rest on a fixed compilation
context that was never verified.

That distinction is where the next result comes from. Four conclusions in these
files fell in one day once the right mechanism arrived, each after two sessions
of sweeping had been written up as proof of unreachability -- see
`docs/near-misses/README.md`. In all four the *measurements* held and the
*context around them* was what moved. So expect the last five to come from a
mechanism or a changed assumption, not from more source-shape sweeping.

### Audit the context before sweeping the source

The questions that have actually paid, in rough order of how often this project
has had them wrong:

1. **Does this function share a translation unit with what I think it does?**
   Unit boundaries are inferred from `$gp` usage and jump-table runs, and both
   inferences have been wrong (see the jump-table section of
   `docs/open-matching-problems.md` for one that was not merely wrong but
   impossible). The boundary decides which constants cse has already seen, what
   loop.c has hoisted, and which allocnos live across a call.
2. **Is every symbol it touches declared at its real type and size?** The
   declaration, not the use, picks symbolic against split addressing, and a
   wrong size silently changes the mode at every site. See
   `docs/addressing-and-translation-units.md`.
3. **Is the `-G` value right for this unit?** `-G8` against `-G0` moves which
   objects are `$gp`-relative, which moves prologue register pressure.
4. **Is the struct layout right?** A field at the wrong offset yields the right
   instruction with the wrong displacement, and the diff then reads as a
   register problem.
5. **Is the function order within the unit right?** It is inferred from address
   order, which is usually but not always the source order.

Only once those are pinned is "the source shape is wrong" the live hypothesis.

**Read the near-miss file before touching one**, and read it the way its own
README says: the measurements are evidence, the verdict is a hypothesis. Each
lists the shapes already measured with their word costs, and most record a
`search.py` sweep that found nothing -- so if the context checks out, the
remaining gap is allocation or ordering rather than source shape.
`python3 tools/decomp/nmcheck.py` compiles all five and checks each against the
count in its own header.

**The mechanism that closed the last four** is still the first thing to try on
the rest: `regs_someone_prefers` makes `find_reg` avoid a register that a
*lower*-priority conflicting allocno prefers, so an allocno allocated first can
be pushed off the register it should have had. The cheapest way to move a
priority is a reference costing no instruction -- splitting one assignment into
two statements that compile to the same single insn. A block-local quantity is priced by the SAME
refs-weighted formula as a global allocno -- this file said otherwise until
2026-10-28; see the correction in section 3 of `docs/matching-levers.md` -- so
both levers apply to it, the free reference and the earlier birth a hoist buys. The conflict side is
a second lever: giving an existing allocno a second role in another block
imports that block's hard-reg conflicts, which is what bought three of
func_80041E70's four words for free. `docs/matching-levers.md` section 3 has the
arithmetic.

## How to work a function

0. `python3 tools/decomp/annotate.py <unit> <func>` -- **do this first.** It
   reads the target listing and prints every lever whose signature is present,
   with the sites, the doc section, the measured word cost of getting it wrong,
   the source shapes to try cheapest-first, and the shapes already measured
   *not* to work. It replaces reading `docs/matching-levers.md` end to end
   (9,000 words) with reading the few entries that can apply. It decides
   nothing -- see the note below on why that is deliberate.
1. Read `asm/nonmatchings/<unit>/<func>.s` -- but get a DRAFT FIRST, because
   hand-decoding a listing is the most expensive step there is:

       make ctx     # once, after any include/*.h change
       python3 vendor/m2c/m2c.py --target mipsel-gcc-c --context ctx.c \
           -f <func> asm/nonmatchings/<unit>/<func>.s

   m2c gets the control flow and most of the arithmetic right and prints the
   extern list it needed, which is most of the declaration hunting done for
   you. Read the listing to CHECK it, not to derive it from scratch. See
   `docs/m2c-pitfalls.md` for where its inferences are wrong -- the types are
   the part to distrust, and it cannot see addressing-mode or allocation
   questions at all.

   **`make ctx` was broken from 2026-10-21 to 2026-10-26** by the same missing
   `-Ipsyq/include` that broke verify.py, so `ctx.c` was stale and m2c emitted
   `?` for every type. If m2c's output has no real type names in it, rebuild
   the context before blaming m2c.
2. Write a candidate into a `zz_*.c` scratch file in the project root
   (gitignored) containing `#include "common.h"`, any prototypes it needs, and
   just that function.
3. `python3 tools/decomp/verify.py zz_foo.c <unit> <func>` -- compiles with the
   real `ccpsx`, assembles through the project's maspsx + gas pipeline, and
   diffs every `.text` word against the reference listing, masking only
   relocated halfwords and 26-bit jump targets. It prints `MATCH` or
   `N DIFF` plus a side-by-side. This is the fast loop; it never touches
   `build/`.
3a. When it says N DIFF, run
   `python3 tools/decomp/advise.py zz_foo.c <unit> <func>` before reading the
   diff. A positional diff is actively misleading once anything has shifted --
   func_8004CECC's single missing register copy reads as "101 DIFF" -- and
   advise.py's verdict tells you which of the two very different jobs you have:
   rewrite the C (SHAPE) or move an allocation priority (ALLOC). It also names
   the six differences that are mechanically diagnosable: signedness, inverted
   if/else arms, base/index reassociation, load-delay nops, a missing
   callee-saved register, and a wrong constant. Each cites the function it was
   measured on.
3b. When the logic is right and only shapes are wrong, hand it to
   `python3 tools/decomp/search.py zz_foo.c <unit> <func>` rather than
   iterating by hand: a verify cycle is 0.7s, so a few hundred variants is
   minutes of compute and no thinking. Add `--only=width,cast,dup` (the
   families `annotate.py` implicated) once the function is big enough that one
   greedy pass is slow. It will not rescue wrong logic, and a candidate it
   flags as closing the word count *at the cost of* differing words wants
   judging by hand -- that is the shape of a semantic accident, not a lever.
4. When it says MATCH, replace the `INCLUDE_ASM` in `csrc/`, run
   `tools/decomp/guard.py` (nothing lost) and `make check` (byte-identical).
5. Commit, with the load-bearing source shapes named in the message and
   commented in the code.

Tools in `tools/decomp/` (they honour `AZURE_ROOT` and `AZURE_SCRATCH`):

| tool | use |
|------|-----|
| `advise.py zz.c UNIT FUNC` | **run this instead of re-reading a diff.** Aligns the two word streams instead of comparing them positionally, so a one-word shift stops reading as N wrong words, and classifies what is left into a KIND with the lever for it. The verdict is the point: SHAPE (the C is wrong, keep rewriting), ALLOC (every opcode and immediate already matches, so only section 3 reaches it) or LENGTH (with the shift localised to a branch span). `--greg` adds the allocator's own refs/live-length/conflict table; `--regs` lists every register-only site |
| `verify.py zz.c UNIT FUNC` | the fast loop above; marks a relocated word whose addend differs from the listing with `?`, and diagnoses the wine case clash |
| `peek.sh UNIT FUNC` | print one already-decompiled function's asm |
| `sizes.py` | every C function whose compiled size differs from its target -- run this first after a failed `make check`, because one wrong size shifts the image and makes every later byte diff meaningless |
| `fdiff.py FUNC...` | built vs reference bytes side by side; only valid after a *successful* link, since a stale `build/slus_006.14` once produced a bogus MATCH |
| `guard.py` | no function was dropped from `csrc/` |
| `dump.sh file.c` | compile with gcc's RTL dumps and collect them |
| `sched.py zz.c FUNC` | the scheduler's own decisions, read out of dump.sh's `sched2`: every insn's priority, the ready list at each cycle, every hazard decision, and `--insn N` to read a ready-list insn number back to an instruction. Run this before blaming a scheduling diff on "the LUID tie-break" -- that is the THIRD key, and section 8 of matching-levers has the order they really apply in |
| `annotate.py UNIT FUNC` | which levers this function's listing can possibly need, with costs and ruled-out shapes; `--sites` to print the matching instructions, `--self-test` to check every signature still fires on a function known to need it |
| `search.py BASE.c UNIT FUNC` | sweep the local source transforms this compiler is sensitive to -- declared widths, `(s16)` casts, splitting a statement in two, `<` against `>`, the `& 1` mask -- scoring each with verify.py. Greedy by default, `--sites` to list the candidate changes, `--exhaustive` when there are few. Prints what each accepted change was worth, which is the table a near-miss header wants |
| `nmcheck.py` | every `docs/near-misses/*.c` still compiles and still agrees with the word/diff count in its own header |

**When a function is one or two words short, compare the internal branches'
immediate fields against the target's**, rather than reading the diff. Every
branch whose span contains the missing word is off by exactly that count, so
the first branch whose delta changes brackets the defect. That localised
func_80042BDC's missing word to one arm of a 180-instruction `case` in a single
verify run, where the differing-word count said nothing useful at all -- it had
200 of 543 words "differing" purely from the shift.

**`annotate.py` deliberately does not choose.** It prints candidates, each
citing its measurement, because the two worst detours in this project came from
inheriting a confident claim rather than from failing to find one -- the "two
sign extensions" story cost two sessions, and "argument preferences are
unreachable" parked two functions on a reason that turned out to be half true.
A tool that ranked confidently would reproduce that failure faster. If you add
an entry, it needs a `cost` naming a real function and word count, and any
shape you measured as useless belongs in its `negative:` list, which is the
most valuable field in the file.

`dump.sh` is the one to reach for when the logic is right and only registers or
the order of two adjacent instructions differ. `cc1psx` honours `-dS` (post
sched1), `-dR` (post sched2), `-dl` (local-alloc), `-dg` (global-alloc) and
`-dr` (initial RTL). `.sched` prints the ready list and each insn's priority
per cycle; `.greg` prints the allocation order, each allocno's conflicts and
its register preferences. Between them they settle a register question without
guessing -- that is how func_80047054's permutation and func_80042A80's one bit
were explained rather than fought.

## Things that cost time once and should not again

- **`make clean` deletes `asm/` and `assets/`, not just `build/`.** The
  `INCLUDE_ASM` macros reference `asm/nonmatchings/**/*.s`, so a plain `make`
  after a clean fails with `can't open asm/nonmatchings/.../func_X.s`. The
  recovery is `make clean && make splat && make`; nothing in the normal build
  regenerates those trees. Do not wire `splat` in as a build prerequisite --
  it re-splits the 26 MB dungeon container and costs minutes per `make`.
- **`make splat` only clears `asm/nonmatchings`, so stale `.s` files survive.**
  `SFILES` is a `find` over `asm/`, so a subsegment you removed or renamed
  leaves its old `.s` behind and it keeps getting linked. If a link starts
  failing on symbols from a segment you deleted, look for orphans in `asm/`.
- **Scratch belongs in `scratch/`.** `.gitignore` covers `scratch/`, `zz*.c`
  and `b[0-9]*.py`; `tools/decomp/verify.py`, `search.py` and `sweep.py` all
  take a path, so `scratch/zz_foo.c` works. The root had accumulated 1252 of
  these before they were moved on 2026-10-26.
- **search.py has already been run over every park within 8 DIFF, and found
  nothing.** 2026-10-27, greedy over its whole transform set on all seventeen
  of them (func_80041E70, func_800599B0, func_80035E58, func_8003A060,
  func_8003A858, func_80056DB4, func_800537D0, func_800565D8, func_8005ACDC,
  func_8003F368, func_8004A574, func_80055D84, func_8003666C, func_80048FBC,
  func_8004437C, func_80049E1C, func_80059DAC): "no single change improved on
  the base" for every one, and not one variant even matched the base. That is the expected answer and worth having in
  writing -- every one of those headers diagnoses its remainder as a
  `local-alloc`/`sched1` tie-break, and the sweep explores SOURCE SHAPES, so
  it cannot reach them. Do not re-run it over these as a first move; read the
  park's own margin numbers (`advise.py --greg`) instead. It is still the
  right first move on a park whose remainder is NOT yet diagnosed.
- **A park file's "measured and not working" list goes STALE, and re-testing it
  is the cheapest landing in the project.** func_80055864 landed on 2026-10-27
  on a spelling its own header had recorded as "3 DIFF" -- measured before the
  other half of the function's shape (six constants, one local each) was
  found, which changed what the allocator did with it. A rejected variant is
  only evidence against the source it was measured on. So after any shape in a
  function changes, re-measure the rejections; and when recording one, say what
  else was in place at the time. `search.py` does this mechanically for the
  families it knows and costs no tokens, so run it over a park before reading
  the header's negatives as final.
- **Declaration decides addressing.** A scalar or an array within `-G8` gets
  the assembler-expanded symbolic form; anything larger or an incomplete
  `extern u16 X[];` gets the compiler split. Check the declaration before
  suspecting the source shape.
- **splat's `%hi`/`%lo` rendering hides the difference** between a symbol
  reference and a plain constant address. A bare `lui reg, 0x8001` is printed
  as `%hi(SYM)` whenever splat can pair the register with a later `%lo`.
  Globals outside the loaded image (0x8002d000..0x8008875f) were constant
  addresses in the original, reached through a constant base pointer with the
  offset written *inside the index expression* -- see func_80043568 in
  `csrc/unk_14934.c` for the idiom. A symbol array with a *variable* index
  always materialises the whole address instead, so declaring such a symbol is
  a dead end (measured; see `docs/near-misses/func_800423C0.c`).
- **decomp-permuter cannot score a function that uses a constant address**:
  splat's `target.o` carries an `R_MIPS_LO16` relocation the constant form
  cannot produce, so the score never reaches zero. `verify.py` and `make check`
  are the authority.
- **Every tool that shells out to `tools/ccpsx` must pass `-Ipsyq/include`,
  not just `-Iinclude`.** `include/common.h` includes `sys/types.h`,
  `libcd.h`, `libgte.h` and `libgpu.h`, which live only under `psyq/include`;
  the Makefile's `CFLAGS` has both paths and the decomp tools had only the
  first. When the Psy-Q types were adopted that made `verify.py`, `search.py`,
  `sweep.py`, `perm.py`, `nmcheck.py`, `dump.sh`, `peek.sh` and
  `permuter_compile.sh` fail on EVERY file at once -- with a misleading
  `gnu/stubs-32.h: No such file or directory` from the host headers rather
  than anything naming the real cause, and `make` still green throughout,
  because the build was never affected. Fixed 2026-10-26; `nmcheck.py` is the
  cheapest check that the fast loop still works at all.
- cc1psx runs under wine and its asm output has CRLF line endings. Strip the
  CR before matching labels.
- **`verify.py` compares only the upper 16 bits of any relocated word.** A
  `%lo(SYM)+N` where the target has `%lo(SYM)` passes, and so does a jump-table
  reference that points at the wrong place. It bit func_8003F80C and it is why
  a MATCH on any of the four switch functions would be misleading. `make check`
  is the only authority; treat a verify MATCH as "worth running make check on".
- **wine's filesystem is case-insensitive, so two scratch files whose names
  differ only in case are the same file to `ccpsx`.** With `zz_gA.s` already in
  the scratch directory, `ccpsx -o .../zz_ga.s` writes `zz_gA.s`, exits 0, and
  `verify.py` then dies with FileNotFoundError on a file the compiler "wrote".
  Name scratch variants so they differ by more than case.

## Open TODOs

`TODO.md` has the live queue. In short:

1. The five near-misses, ordered above. **func_80041E70 at one word is the
   closest**, though its bound is the best-argued of the five; func_8003F368 at
   4 and func_8004437C at 8 are the next. Run the context audit above before
   sweeping any of them. The two functions that fell on 2026-10-23 both went to
   a mechanism read out of gcc 2.7.2's source rather than to a source sweep,
   which is the pattern to repeat.
2. A decision on func_8003E758's decomp device. It ships
   `__asm__ volatile ("" :: "r" (q) : "memory")`, worth 13 words, which shapes
   the allocator directly rather than reproducing a source shape. The honest
   fallback is 698 words / 11 DIFF with the memory clobber alone.
3. func_80042BDC's load-bearing source shapes were never written up -- the
   agent that closed it was cut off immediately after reporting. The code
   comment says so; the 14-DIFF near-miss is in git history if they need
   recovering.
4. Deferred type merges, all recorded where they matter: four scratchpad
   typedefs (Unk3DBD0SP/Unk463ECSP/Unk44D24SP/Unk3CCB0SP), Unk800E3E48 into
   Unk42710, and the RotAverage4 prototype disagreement (`s32 *` against
   `u32 *`). Each is held back for a stated reason -- usually that retyping a
   field risks an existing match for a cosmetic gain -- so read the reason
   before doing one. The three hand-written POLY_FT4s
   (Unk463ECP/Unk3D0F0P/Unk44D24P) are DONE: the merge resolved itself by
   adopting psyq's own POLY_FT4 in func_800463EC, func_8003D0F0 and
   func_80044D24. All three still match, so the "retyping would change
   func_800463EC's RotAverage4 argument types" worry those entries cited did
   not materialise; the separate prototype disagreement in this list is
   untouched.
5. The overlays, which are where the actual game is. `docs/overlays.md` has the
   analysis; nothing there is started.
6. `tools/lib2ar.sh` references a path under `$HOME/scratch` and does
   `basename $1 .LIB` against a lowercase `.lib`. Known, tangential.

## Running agents on this

Fresh `general-purpose` agents pointed at `docs/matching-levers.md` are the
cheap way to work a parked function: measured at 212K tokens / 47 tool calls
for func_80044D24, against about 890K / 326 for a fork of the main session,
which inherits the whole conversation and pays for it on every turn. Give the
agent the function, the three documents to read first, the mechanism you think
it needs, and an explicit instruction not to run `make splat`, not to touch
`csrc/`, and not to commit -- integration is the parent's job.

Do not kill an agent that has been running a while: everything it has measured
is in its context and is lost. Send it a message asking it to report what it
has now, which caps its remaining turns and still yields the measurements.
