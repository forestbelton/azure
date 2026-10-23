# TODO

See `docs/where-we-are.md` for the state and `docs/matching-levers.md` for how
this compiler behaves. **186 of 191 resident-executable functions are C and
`make check` is byte-identical.** Both of the structural blockers that shaped
most of this project -- jump-table placement and the object reached both
`$gp`-relatively and absolutely -- are solved and neither needed the thing it
was being built towards; see `docs/where-we-are.md`.

The queue, in order:

- [ ] **The five near-misses.** All five have their source written and checked
      (`python3 tools/decomp/nmcheck.py`); between them they are 1, 4, 8, 30 and
      62 differing words, and four differ only in which register holds what.
      `docs/near-misses/` has the source and the ruled-out shapes for each, and
      `docs/where-we-are.md` orders them.

      func_800453E0 and func_80045CC4 both landed on 2026-10-23, and HOW they
      landed is the thing to repeat: both went to a mechanism read out of gcc
      2.7.2's own source, not to a source sweep. The sources are at
      `/tmp/claude-1000/.../scratchpad/gccsrc/gcc-2.7.2/` or re-fetch from
      ftp.gnu.org/old-gnu/gcc/. Two cautions found the hard way: upstream 2.7.2
      has NO regmove.c (the Cygnus build carries passes upstream lacks, though
      regmove is a measured no-op on every function tested), and reading the
      source corrected three claims this project had been reasoning from.

      **Before sweeping any of them, run the context audit** in
      `docs/where-we-are.md`. A C source producing these bytes exists by
      construction, so every "unreachable from C" in these files really means
      "unreachable given the unit boundary, declared sizes, `-G` value, struct
      layout and function order this project has inferred" -- and those
      inferences are where four previous dead ends turned out to be.

- [x] **DECIDED 2026-10-23: func_8003E758's decomp device STAYS** until a more
      robust solution turns up. Left in place deliberately, not overlooked; the
      honest fallback is recorded below and in the function's comment if anyone
      wants to take the 11 DIFF instead. Original note follows.

      It ships
      `__asm__ volatile ("" :: "r" (q) : "memory")`, worth 13 words (3 reload,
      1 nop placement, 9 a register swap via a free allocno reference). It
      shapes the allocator directly rather than reproducing a source shape.
      The honest fallback is 698 words / 11 DIFF with the memory clobber
      alone. Owner's call; flagged in the source comment and the commit.

- [ ] **Write up func_80042BDC's load-bearing shapes.** The agent that closed
      it was cut off immediately after reporting, so only the source survives.
      The code comment says as much; the 14-DIFF near-miss is in git history
      if the shapes need recovering by diff.

- [ ] **Deferred type merges**, each recorded where it matters and each held
      back for a stated reason -- usually that retyping a field risks an
      existing match for a cosmetic gain. Read the reason before doing one.
      Four scratchpad typedefs for the same 0x1F800000 region
      (`Unk3DBD0SP`, `Unk463ECSP`, `Unk44D24SP`, `Unk3CCB0SP`); three
      POLY_FT4s (`Unk463ECP`, `Unk3D0F0P`, `Unk44D24P`); `Unk800E3E48` into
      `Unk42710`; and the `RotAverage4` prototype disagreement (`s32 *`
      against `u32 *`).

- [ ] **The overlays**, which are where the actual game is: roughly 8.4 MB of
      code against the resident executable's 216 KB. `docs/overlays.md` has
      the analysis. `dungeon.bin` has no splat spec at all, `town.bin` has one
      `bin` blob over all 16 MB, and `main.bin`'s 28 subsegments are all
      `bin`. Nothing here is started.

- [ ] Tool that scans `splat/*_symbol_addrs.txt` and warns about unused
      symbols.

- [ ] **Work out what the ~120 KB of unidentified CODE in slus_006.14 is.**
      23.6% of the file, about three times the volume of decompiled game code;
      see the composition table in `docs/where-we-are.md`. **This is NOT the
      mechanical job it was filed as -- investigated 2026-10-23 and the easy
      answer is ruled out.**

      What was checked. `azure libmatch psyq` confirms the SDK release is
      right: **162 of 163 already-placed modules match EXACTLY, mean fuzzy
      0.998**, over libapi, libc, libcard, libcd, libds, libetc, libgpu,
      libgte, libsn, libsnd and libspu. So the library set in `psyq/lib` is the
      one the game linked against. But `azure libscan psyq` over the two large
      unidentified code blocks -- 0x72a8-0x101f8 (36 KB, 348 prologues, sitting
      immediately before `main`) and 0x1c540-0x301a8 (81 KB, 625 prologues) --
      locates **zero** modules out of 1257 examined, at `--min-invariant` 4 and
      12 alike. The only hits are a handful of tiny stubs matching ambiguously
      at many sites on 4-8 invariant words, which is noise.

      **ANSWERED by reading it: it is ~421 functions, and not library code.**
      Block A (0x072a8-0x101f8) holds 113 functions, block B
      (0x1c540-0x301a8) holds 308, counted by bucketing every `jal` in the
      image by call-site region. Both are cohesive (232 and 580 internal
      calls) and both are called by already-decompiled game code (15 and 42
      times). The first function in block A is a bitmap set/clear/test triple
      over a table at 0x80012D70, and its relocation-free instruction run
      appears twice in the executable and in NONE of the twenty .lib files --
      while content matching provably works, since libmatch placed 162 modules
      that way. Full write-up in `docs/where-we-are.md`.

      So the real work here is splitting and decompiling them, not scanning:
      **this executable has roughly three times as many functions as the 191
      this project has been counting.** Still open: whether they are the game's
      own code or a third-party library linked from outside psyq/lib.

      NOTE, since this entry previously said otherwise: `psyq-obj-parser` is
      **not** needed for any of this. `libmatch`/`libscan` read `.lib` files
      natively through `src/azure/psyobj.py`. The parser only serves
      `tools/lib2ar.sh`, i.e. manual `nm`/`objdump` introspection.
