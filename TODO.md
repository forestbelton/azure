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

- [ ] **A decision on func_8003E758's decomp device.** It ships
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

- [ ] `tools/lib2ar.sh` references a path under `$HOME/scratch` and does
      `basename $1 .LIB` against a lowercase `.lib`. Known, tangential.
