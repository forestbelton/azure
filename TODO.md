# TODO

See `docs/where-we-are.md` for the state and `docs/matching-levers.md` for how
this compiler behaves. The queue, in order:

- [ ] The six parked near-misses, best-first. `docs/where-we-are.md` orders
      them and `docs/near-misses/` has the source and the ruled-out shapes for
      each. Two have a mechanism named and not yet applied, and are the
      cheapest real progress available: func_80045CC4 (1 word of 437, with a
      0.9% register-priority gap measured) and func_80042900 (one duplicated
      `move`). Two others (func_80047054, func_800407C0) are argued unreachable
      from C and are recorded so nobody re-opens them.
- [ ] **The jump-table spec change**, which unblocks about 1550 instructions
      across func_80041344, func_80041588, func_80042BDC and func_8003E758.
      `docs/open-matching-problems.md` has the full analysis; it needs
      `csrc/unk_14934.c` split in two at func_80041588, the opening bin blob
      carved, linker-script placement of three units' `.rodata`, and all three
      unk_14934 switch functions landed together.
- [ ] A decision on the 31 addressing-blocked functions. This is about where
      the project's data lives, so it is the owner's call; see
      `docs/addressing-and-translation-units.md`.
- [ ] Two cheap spec changes noted in `docs/where-we-are.md`: extending the
      `main` asm subsegment to 0x102c4 (which disposes of func_8003CAA8, not
      actually a function), and carving out func_8003F5E0 and the adjacent
      func_80044618/func_80044698 so their `-G8` assembly is possible.
- [ ] Tool that scans `splat/*_symbol_addrs.txt` and warns about unused symbols.
