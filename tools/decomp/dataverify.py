#!/usr/bin/env python3
"""dataverify.py FILE -- check candidate DATA definitions against the image.

`ovlverify.py`'s job for an overlay's `.data`. Compile FILE at the overlay's
own `-G0`, take the object's `.data` section, and compare it byte for byte
against the container the definitions claim to reproduce -- without a link, so
an agent can run it, and without `make`, so it cannot disturb a build.

FILE is a standalone C file holding only definitions (plus whatever
declarations they need). Every object must be named `D_<vram>`: the vram is
read out of the name, which is what lets the tool find the reference bytes and
check the layout without being told anything.

THREE CHECKS, and the second is the one that matters:

1. **Bytes.** Each object's bytes against `data/town/town.bin` at
   `rom = win_rom + (vram - win_vram)`. A word carrying a relocation reads 0
   in an unlinked object, so those words are MASKED and listed separately --
   same bargain `ovlverify.py` makes, and the same obligation: read each
   relocation's symbol against the listing yourself.

2. **Layout.** Each object's offset inside `.data` must equal its vram minus
   the first object's vram. This is what catches a wrong array length, and it
   catches it AT THE OBJECT rather than as a mystery 0x60 at the end. Get it
   wrong in the yaml instead and `make` reports `size differs by +0x60` with
   the first difference inside the overlay's RODATA, which looks like a rodata
   bug and is not -- see docs/owning-data.md §8.

3. **Window.** The printed `.data` size is the number the yaml's next
   subsegment boundary must be, as `win_rom + size`. Paste it into the
   `[..., data, ovl_town_data]` line and nowhere else.

What this CANNOT see, for the same reason ovlverify cannot: a relocation
against the wrong symbol that happens to resolve to the right word, and a
declaration mismatch that only the real unit would hit. `make containers` is
still the gate, and it is mine, not the agent's.

    python3 tools/decomp/dataverify.py scratch/dq1_defs.c
    python3 tools/decomp/dataverify.py scratch/dq1_defs.c --window 0x51fc0:0x800CF720
"""
import argparse
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, HERE)
import asmlib

# The windows a `.data` subsegment can currently be opened on, as
# name -> (rom offset of the region, vram of the region, reference container).
WINDOWS = {
    "ovl_town": (0x51FC0, 0x800CF720, "data/town/town.bin"),
    "ovl_dungeon": (0xF7700, 0x800DCE60, "data/dungeon/dungeon.bin"),
}


def object_data(o):
    """(bytes, {offset: R_MIPS_*}, [(offset, name)]) for an object's .data."""
    raw = b""
    for ln in subprocess.run(
            ["mips64-linux-gnu-objdump", "-s", "-j", ".data", o],
            capture_output=True, text=True).stdout.splitlines():
        m = re.match(r"\s*([0-9a-f]+)\s((?:[0-9a-f]{2,8}\s){1,4})", ln)
        if m:
            raw += bytes.fromhex("".join(m.group(2).split()))
    relocs = {}
    for ln in subprocess.run(
            ["mips64-linux-gnu-objdump", "-r", "-j", ".data", o],
            capture_output=True, text=True).stdout.splitlines():
        m = re.match(r"([0-9a-f]+)\s+R_MIPS_(\w+)\s+(\S+)", ln)
        if m:
            relocs[int(m.group(1), 16)] = (m.group(2), m.group(3))
    # `nm -S` prints NO size for these: gas emits no `.size` for a data
    # label, so the sizes have to come from the gaps -- which is the same rule
    # the listing's dlabels follow, so it is the right one anyway.
    off = []
    for ln in subprocess.run(
            ["mips64-linux-gnu-nm", "--defined-only", o],
            capture_output=True, text=True).stdout.splitlines():
        p = ln.split()
        if len(p) == 3 and p[1] in "dD":
            off.append((int(p[0], 16), p[2]))
    off.sort()
    syms = [(a, (off[i + 1][0] if i + 1 < len(off) else len(raw)) - a, n)
            for i, (a, n) in enumerate(off)]
    return raw, relocs, syms


def main() -> None:
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("file")
    ap.add_argument("--window", default="ovl_town",
                    help="a name from the table, or ROM:VRAM (default ovl_town)")
    args = ap.parse_args()

    if ":" in args.window:
        a, b = args.window.split(":")
        rom0, vram0, ref = int(a, 0), int(b, 0), WINDOWS["ovl_town"][2]
    else:
        rom0, vram0, ref = WINDOWS[args.window]
    blob = open(os.path.join(PROJ, ref), "rb").read()

    try:
        asm = asmlib.compile_asm(args.file, cflags=("-G0",))
    except asmlib.CompileError as e:
        raise SystemExit("compile failed:\n" + str(e))
    o = asmlib.assemble(asm, os.path.splitext(os.path.basename(args.file))[0] + "_data")
    raw, relocs, syms = object_data(o)

    if not syms:
        raise SystemExit("no .data symbols: every object needs an explicit "
                         "initialiser, or cc1psx puts it in .bss")
    bad = [n for _, _, n in syms if not re.fullmatch(r"D_[0-9A-Fa-f]{8}", n)]
    if bad:
        raise SystemExit("not named D_<vram>, so the reference cannot be "
                         "found: " + ", ".join(bad))

    base = int(syms[0][2][2:], 16)
    print(f"{args.file}: {len(syms)} objects, .data {len(raw)} bytes ({len(raw):#x})")
    print(f"  window {rom0:#x}/{vram0:#010x}; first object D_{base:08X}")
    print(f"  NEXT YAML BOUNDARY: {rom0 + (base - vram0) + len(raw):#x}")
    if base != vram0:
        print(f"  NOTE: the first object is not at the window start {vram0:#010x}; "
              f"a C unit's .data must be the region's PREFIX (§8)")

    print(f"\n  {'object':12} {'size':>6} {'layout':>8}  bytes")
    fails = 0
    for off, size, name in syms:
        vram = int(name[2:], 16)
        want_off = vram - base
        lay = "ok" if off == want_off else f"+{off - want_off:#x}"
        if off != want_off:
            fails += 1
        rom = rom0 + (vram - vram0)
        got = raw[off:off + size]
        tgt = blob[rom:rom + size]
        nrel = 0
        diff = []
        for i in range(0, size - 3, 4):
            if off + i in relocs:
                nrel += 1
                continue
            if got[i:i + 4] != tgt[i:i + 4]:
                diff.append(i)
        tail = size - (size // 4) * 4
        for i in range(size - tail, size):
            if got[i:i + 1] != tgt[i:i + 1]:
                diff.append(i)
        if diff:
            fails += 1
        state = "OK" if not diff else f"{len(diff)} DIFFER at +{diff[0]:#x}"
        print(f"  {name:12} {size:>6} {lay:>8}  {state}"
              + (f"  ({nrel} relocated word{'s' * (nrel != 1)} masked)" if nrel else ""))

    if relocs:
        print("\n  relocations -- check every symbol against its own listing:")
        for off in sorted(relocs):
            kind, sym = relocs[off]
            owner = max((s for s in syms if s[0] <= off), key=lambda s: s[0])
            print(f"    {owner[2]}+{off - owner[0]:#x}  R_MIPS_{kind}  {sym}")

    print(f"\n{'OK' if not fails else 'FAIL'}  {fails} problem(s)")
    sys.exit(1 if fails else 0)


if __name__ == "__main__":
    main()
