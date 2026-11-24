#!/usr/bin/env python3
"""symsafe.py -- which real names a unit may use, and which it may NOT.

THE RULE. `splat/common_syms.txt` holds names for the RESIDENT library and
BIOS routines -- every one of its 219 entries is in 0x8003c9f8..0x8006a3a8,
below the overlay slot -- so any unit in any container may call them by name.
The per-container files (`splat/<container>_symbol_addrs.txt`) are different:
they name symbols at addresses INSIDE THE SHARED OVERLAY SLOT, and an address
in that slot means different code depending on which overlay is mapped.

So `plt_dm_kicked = 0x80098A98` is a true name of town.bin's code and a LIE
in a dungeon unit: when DUNGEON.BIN is loaded, 0x80098A98 is dungeon code.
Using it there would link -- the symbol exists -- and would be wrong at
runtime, and nothing in the build or the byte-exact verification could
possibly catch it, because the bytes are identical either way. The name is
the only thing that is false.

This is why the agent brief says to check `common_syms.txt` specifically and
not "the symbol files". A rename pass that read the per-container files would
have introduced exactly this.

    python3 tools/decomp/symsafe.py            # audit every unit in csrc/
    python3 tools/decomp/symsafe.py --slot     # print the slot bounds it uses
"""
import argparse
import glob
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.path.dirname(os.path.dirname(HERE))
PROJ = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# The RAM window the per-container overlays share, read off
# splat/dungeon.bin.yaml's code segment (vram 0x80088760, text end 0xf7700).
SLOT_LO, SLOT_HI = 0x80088760, 0x800F7700


def container_of(path):
    """Which container a csrc file builds into, by its name."""
    b = os.path.basename(path)
    if b.startswith(("ovl_dng_", "ovl_dungeon")):
        return "dungeon.bin"
    if b.startswith("ovl_town"):
        return "town.bin"
    if b.startswith("ovl_main"):
        return "main.bin"
    return None          # resident / shared: no slot conflict to check


def named(src):
    out = {}
    for ln in open(src):
        m = re.match(r"(\w+) = (0x[0-9a-fA-F]+);", ln.strip())
        if m and not m.group(1).startswith(("func_", "D_", "jtbl_", "_")):
            out.setdefault(int(m.group(2), 16), m.group(1))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--slot", action="store_true")
    a = ap.parse_args()
    if a.slot:
        print("shared overlay slot: %#x .. %#x" % (SLOT_LO, SLOT_HI))
        return 0

    sys.path.insert(0, HERE)
    import dngclone

    # per-container names that sit in the slot: valid ONLY in their own container
    risky = {}
    for f in glob.glob(os.path.join(PROJ, "splat", "*_symbol_addrs.txt")):
        cont = os.path.basename(f).replace("_symbol_addrs.txt", "")
        cont = cont if cont.endswith(".bin") else cont
        for addr, name in named(f).items():
            if SLOT_LO <= addr < SLOT_HI:
                risky.setdefault(name, set()).add(cont)

    bad = 0
    for path in sorted(glob.glob(os.path.join(PROJ, "csrc", "**", "*.c"),
                                 recursive=True)):
        cont = container_of(path)
        if cont is None:
            continue
        text = open(path).read()
        mask = dngclone.code_mask(text)
        for name, owners in risky.items():
            if cont.replace(".bin", "") in {o.replace(".bin", "") for o in owners}:
                continue           # its own container: legitimate
            for m in re.finditer(r"\b%s\b" % re.escape(name), text):
                if mask[m.start()]:
                    print("%s (%s) uses %s, which names %s's code at an "
                          "address in the shared slot"
                          % (os.path.relpath(path, PROJ), cont, name,
                             "/".join(sorted(owners))))
                    bad += 1
    print("%d cross-container slot name(s) in code" % bad)
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
