# TODO

See `docs/where-we-are.md` for the state and `docs/matching-levers.md` for how
this compiler behaves. **186 of 1008 resident-executable functions are C and
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

- [ ] **The overlays**, which are where the actual game is and are now the
      whole of the remaining work. `docs/overlays.md` has the analysis.

      **The earlier text of this entry was stale** -- it said dungeon had no
      spec, town was one 16 MB blob and nothing was started. Four code
      overlays are split, in the build and byte-identical: `main.bin`'s live
      one at `0x80020000` (241 functions / 7,487 words), `main.bin`'s debug
      twin at `0x80400000` (263 / 8,271), `town.bin`'s at `0x80088760`
      (1,815 / 70,244) and `dungeon.bin`'s at the same address (862 /
      85,558). Town's is 1.7x this executable's 41,767 words and dungeon's
      2.0x.

      **Decided 2026-11-01: start with `main.bin`'s card overlay at
      `0x80020000`.** It is one eighth of what was just finished, it is a
      coherent self-contained subsystem (memory card save/load), it is the
      only overlay with a translation-unit oracle, its load address collides
      with nothing (town and dungeon share `0x80088760`, so an address there
      means different code depending on which is resident), and it makes
      `main.bin` container-complete. It also establishes the per-overlay
      C build pattern on 7.5K words instead of discovering it on 85K.

      **Started 2026-11-01. `main.bin`'s overlay is a `c` subsegment, 3 of
      241 functions, all four containers byte-identical.** The flags are
      settled: cc1psx `-O2 -G0` with maspsx at `-G0`, overridden per unit in
      the Makefile. `-G0` is forced, not chosen -- GPREL16 reaches +/-32K
      from `$gp` and no overlay keeps its data in that window -- and it is
      what gives the hoisted-lui global access shape instead of the
      assembler-expanded `lui $at` pair.

      **The twin is the same source at `-G8`.** One C source reproduces
      `func_80020924` (24 words, `-G0`) and `func_804008A0` (26 words, `-G8`)
      with 0 differing words each, so every function here has TWO independent
      targets and a shape that matches one build but not the other is wrong.
      `tools/decomp/ovlverify.py --G` checks either channel.

      The unit map is in `docs/overlays.md`: nine original translation units,
      `tools/assertmap.py` recovers them, and the live block's own 24 assert
      sites (all `c_server.c`) agree with the propagated map wherever they
      overlap.

      **Sweeping: 220 of 241 functions, 6,457 of 7,487 words (86.2%) as of
      2026-11-01.** Every function in the unit has now been worked; the 28
      remaining stubs are all near-misses with measurement tables in
      `docs/near-misses/`. The six-function family that shared one residue was
      cracked by decomp-permuter: give it a base whose SHAPE is right and let
      it search orderings. Work the FAMILIES -- grouping stubs by masked
      instruction signature found 15 of them, and decoding one member of a
      repeated family makes the rest nearly free: five input handlers, 527
      words, came from one decode. `tools/decomp/ovlverify.py` is the fast
      loop and `make build/main.bin` is the gate.

      The lever that matters most for the NEXT overlay is that
      `func_80069DF8` is `noreturn`: without that declaration every assert
      site costs a wasted word, and with it the overlay's two assert-bearing
      functions (378 words, one of them 264) fell out at once. `town.bin` has
      76 assert records to this overlay's 10.

      **The carve is NOT the first step, which is a correction to the plan
      this entry used to carry.** Only one of the nine boundaries is anywhere
      near pinned (`c_server.c` ends at idx 57 or 58, from the rodata
      ordering constraint), carving needs nine exact text boundaries plus a
      three-way rodata split, and at `-G0` unit membership no longer changes
      the shape of a global access. The sweep produces the anchors that pin
      the carve, not the other way round. One combined file is safe for
      rodata order as long as function order is preserved.

      One near-miss parked with its analysis in the source:
      `func_80022B48` and five siblings, 40 words each, three spellings
      measured and none reaching it.

      Open before town can follow: its 59 tier-2 low-RAM overlays vote much
      more weakly than the big four (`+0x2ab000` 42 against 9, `+0x64b000`
      only 12 against 3), and 54 copies of `main.c` across the container
      suggest many separate links of one source, which has to be checked.

- [ ] **The data in `slus_006.14`.** `docs/data-map.md` is the accounting and
      the method; read it before touching any `bin` blob. State as of
      2026-10-30: splat reports 122,048 bytes unknown, down from 299,816,
      because **176,128 bytes of the file turned out not to be part of the
      executable at all** -- the PS-X EXE header's `t_size` puts the end of
      the loaded image at file 0x55000, and the rest is mastering slack. That
      is now the `tail_unloaded` subsegment and it needs no further work.

      Of what is left: 5,556 bytes in five blobs is library **code**, not
      data; ~81 KB is the structured blob at 0x3E01C (part of which is a
      1025-entry sine table at 0x4EACC); and ~35 KB is Shift-JIS text.

      **Done 2026-10-30: 0x2810-0x57F8**, the item description-and-name pool,
      322 strings under `str/item/`. Two things it established, both in
      `docs/data-map.md`: subsegment boundaries must satisfy
      `offset % 4 == 0` because `subalign: 4` pads every section, and
      `.shiftjis` now escapes the script's control bytes (0x0A, 0x11, 0x08)
      -- that second one was blocking *every* text pool and is now cleared.

      Next, in order: **0xEFC-0x2164** (4,712 bytes, 54 uncovered non-zero),
      **0x800-0xDC0** (1,472, 70), then the 90 strings left at 0x57F8, which
      need one extra segment per 2-aligned entry. Use the union-plus-zero
      coverage test rather than one detector's output: `azure findstr` misses
      kanji runs -- it skipped 46 untranslated Japanese item names in the pool
      just split -- and Ghidra's string model beats both local scanners.

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

      **DONE: all four code blocks are carved -- 817 of the game's own
      functions, and the code accounting is now complete.** unk_72A8 (294),
      unk_1C540 (465), unk_1AF38 (53) and unk_301D4 (5). Every remaining bin
      range was surveyed for prologues, returns and inbound calls; only 188
      bytes of code were left and they are carved. The rest of the "unknown"
      300 KB is data. The executable has 1008 functions, 186 of them C. `unk_72A8` (294) and `unk_1C540` (465) are C subsegments as
      of 2026-10-23 and `make check` is byte-identical. They are game code, not
      SDK modules: block A calls AddPrim, GetTPage, ClearImage, RotTransPers4,
      RotMatrix, CompMatrix, PushMatrix/PopMatrix, ScaleMatrix, LoadImage and
      VSync; block B calls DrawSync, SetSemiTrans, the matrix stack, bzero,
      memcpy, strcpy, strncpy, strrchr and Enter/ExitCriticalSection -- the SDK
      being consumed, in the same vocabulary as the engine code already
      decompiled. **The remaining work is decompiling 764 stubs**, which is now
      the project's main body of work by a wide margin.

      Earlier notes in this entry, kept for the record:
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
