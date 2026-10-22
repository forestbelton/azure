#!/usr/bin/env python3
"""Emit everything this project knows about symbols, for Ghidra to import.

Writes a TSV of name/address/kind. Doing the parsing here rather than in the
Ghidra script keeps the awkward side (Jython vs PyGhidra, Python 2 vs 3) down
to reading three columns.

Sources:
  * splat's symbol_addrs      -- the hand-curated PSY-Q library symbols
  * asm/nonmatchings/*/*.s    -- one file per game function, named for its
                                 vram address, so the address is in the name
"""

import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
OUT = ROOT / "build" / "ghidra_symbols.tsv"
GP_VALUE = 0x80080994  # set by crt0 at 0x800339ac, not by the PS-EXE header


def from_symbol_addrs(path):
    out = []
    if not path.exists():
        return out
    for line in path.read_text().splitlines():
        body, _, comment = line.partition("//")
        body = body.strip()
        if "=" not in body:
            continue
        name, _, value = body.partition("=")
        try:
            addr = int(value.strip().rstrip(";"), 16)
        except ValueError:
            continue
        kind = "func" if "type:func" in comment else "label"
        out.append((name.strip(), addr, kind))
    return out


def from_nonmatchings(root):
    out = []
    for f in sorted(root.glob("*/*.s")):
        m = re.fullmatch(r"func_([0-9A-Fa-f]{8})", f.stem)
        if m:
            out.append((f.stem, int(m.group(1), 16), "func"))
    return out


def main():
    syms = from_symbol_addrs(ROOT / "splat/slus_006.14_symbol_addrs.txt")
    nm = ROOT / "asm/nonmatchings"
    game = from_nonmatchings(nm) if nm.is_dir() else []
    if not game:
        sys.stderr.write("note: no asm/nonmatchings yet -- run `make splat`\n")

    seen = {}
    for name, addr, kind in syms + game:
        seen.setdefault(name, (addr, kind))
    seen["_gp"] = (GP_VALUE, "label")

    OUT.parent.mkdir(parents=True, exist_ok=True)
    with open(OUT, "w") as fh:
        fh.write("# name\taddress\tkind\n")
        for name in sorted(seen, key=lambda n: seen[n][0]):
            addr, kind = seen[name]
            fh.write("%s\t0x%08x\t%s\n" % (name, addr, kind))

    funcs = sum(1 for v in seen.values() if v[1] == "func")
    print("wrote %s: %d symbols (%d functions, %d labels)"
          % (OUT.relative_to(ROOT), len(seen), funcs, len(seen) - funcs))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
