#!/usr/bin/env python3
"""verify.py's job for an OVERLAY function.

An overlay still split as `asm` is ONE big listing with every function in it,
and `tools/decomp/verify.py` cannot read that -- it wants
`asm/nonmatchings/<unit>/<func>.s`, which splat emits only for a `c`
subsegment. So this tool takes the target from whichever of the two exists:
the per-function listing when the overlay has been converted to `c`, and the
single listing otherwise. Either way it supplies the -G0 the overlays need,
which is the other half of why verify.py is not enough.

The overlays are -G0, NOT the -G8 the resident executable is built with, and
that is not a style choice: at -G8 cc1psx records `.extern SYM,4` and the
access comes out as the assembler-expanded `lui $at` / `%lo` pair, while at -G0
cc1psx emits its own `%hi`/`%lo` split address into a general register -- the
hoisted-lui shape the live overlay uses. Measured on func_80020924: -G8 gives
26 words with two `lui $at`, -G0 gives 25 with `lui $v1`, and the target is 24.
(The debug twin at 0x80400000 uses the `lui $at` shape throughout, so IT looks
like a -G8 build of the same source. Do not assume one -G fits both blocks.)

ONE FALSE POSITIVE TO KNOW ABOUT: run on a function that is still an
INCLUDE_ASM stub, this reports 0 differing. The stub's own assembly defines
the symbol in the object, so the candidate IS the target. A 0 only means
something for a function you have actually written in C.

WHAT THIS CANNOT SEE, and it has already cost one broken build: it compares
ONE FUNCTION'S words. It says nothing about the unit's .rodata. A C function
with a local array initialiser emits that initialiser into the unit's
.rodata, and if the same bytes are also pulled in by an INCLUDE_RODATA line
the unit gets them twice -- which happened with D_800200A8 and
func_800258C8, put 0x10 extra bytes ahead of the text, and moved every
function from func_800210F0 on by 0x10. Every per-function check here still
reported 0 differing; only `make build/main.bin` caught it. So a clean run of
this tool is a reason to build, not a substitute for building.

Usage:
    python3 tools/decomp/ovlverify.py <candidate.c> <func_8002xxxx> [more funcs]
    python3 tools/decomp/ovlverify.py --dump <func_8002xxxx>
"""
import argparse
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import asmlib  # noqa: E402

PROJ = asmlib.PROJ
OVERLAYS = [
    ("ovl_main_80020000", 0x80020000, 0x80027E20),
    ("ovl_main_80400000", 0x80400000, 0x804089E0),
    # Town's text starts at 0x8008AC84, NOT the 0x8008AD90 the prologue
    # fingerprinting reported: four frameless leaf functions build no stack
    # frame and so cast no vote. See splat/town.bin.yaml.
    ("ovl_town", 0x8008AC84, 0x800CF720),
    ("ovl_dungeon", 0x80089588, 0x800DCE60),
]
ROW = re.compile(r"\s*/\* \w+ (\w+) ([0-9A-F]{8}) \*/\s+(.*)")


def overlay_for(func):
    a = int(func.split("_")[1], 16)
    for name, lo, hi in OVERLAYS:
        if lo <= a < hi:
            return name
    raise SystemExit(f"{func} is in none of the split overlays")


def listing(func):
    """The listing holding `func`: per-function if the overlay is `c`, else the
    overlay's single `asm` listing. A per-function listing can open with a
    migrated rodata block, but that uses `dlabel`, so the `glabel` scan below
    is unaffected."""
    ovl = overlay_for(func)
    per_fn = os.path.join(PROJ, "asm", "nonmatchings", ovl, func + ".s")
    if os.path.exists(per_fn):
        return per_fn
    # An overlay can be split across SEVERAL translation units, and then its
    # listings live under the unit's directory, not the overlay's --
    # overlay_for() maps an address range to ONE name and cannot know that.
    # ovl_town was carved into three units on 2026-11-03 and every function in
    # the carved range stopped resolving. Search the sibling unit directories
    # before giving up.
    import glob as _glob
    for cand in sorted(_glob.glob(os.path.join(
            PROJ, "asm", "nonmatchings", ovl + "_*", func + ".s"))):
        return cand
    whole = os.path.join(PROJ, "asm", ovl + ".s")
    if os.path.exists(whole):
        return whole
    raise SystemExit(f"no listing for {func}: not in {ovl} or any {ovl}_* unit, "
                     f"and no {whole}")


def target(func):
    """[(word, text)] for one function, out of its overlay listing."""
    path = listing(func)
    out, inside = [], False
    for ln in open(path):
        if ln.startswith("glabel "):
            inside = ln.split()[1].strip() == func
            continue
        if ln.startswith("endlabel") and inside:
            break
        if not inside:
            continue
        m = ROW.match(ln)
        if m:
            out.append((int.from_bytes(bytes.fromhex(m.group(2)), "little"),
                        m.group(3).strip()))
    if not out:
        raise SystemExit(f"{func} not found in {path}")
    return out


RELOC = re.compile(r"%(?:hi|lo)\(|^(?:jal|j)\s+(?:func_|\.L)")


def norm(w, text, relocated):
    """Mask ONLY the fields the linker still has to fill in.

    An unresolved `jal` or `%hi`/`%lo` in a freshly assembled object reads 0,
    so comparing those raw reports every global access as wrong. But masking
    every i-type immediate, which this did at first, also hides REAL
    displacement differences: func_800240EC came back "1 differing" when its
    cursor started at p + 0x260 with displacement 0 against the target's p
    with displacement 0x260 -- every load and store differed and only the
    `addiu`-against-`addu` showed. So the mask is now driven by whether that
    particular word carries a relocation, taken from the candidate object's
    own reloc table and from the target listing's `%hi`/`%lo`/symbolic-target
    text.
    """
    if not relocated:
        return w
    op = w >> 26
    if op in (2, 3):
        return 0x0C000000 if op == 3 else 0x08000000
    return w & 0xFFFF0000               # lui / i-type: drop the imm16


def report(func, got, gotrel, tgt, masked):
    gw = [w for _, w, _ in got]
    tw = [w for w, _ in tgt]
    if masked:
        # A pair is masked when EITHER side carries a relocation: the
        # candidate's comes from objdump's reloc table, the target's from the
        # listing text splat wrote.
        trel = [bool(RELOC.search(t)) for w, t in tgt]
        grel = [a in gotrel or bool(RELOC.search(t)) for a, w, t in got]
        n = max(len(gw), len(tw))
        rel = [(grel[i] if i < len(grel) else False)
               or (trel[i] if i < len(trel) else False) for i in range(n)]
        gc = [norm(w, t, rel[i]) for i, (_, w, t) in enumerate(got)]
        tc = [norm(w, t, rel[i]) for i, (w, t) in enumerate(tgt)]
    else:
        gc, tc = gw, tw
    diff = sum(1 for i in range(max(len(gc), len(tc)))
               if (gc[i] if i < len(gc) else None)
               != (tc[i] if i < len(tc) else None))
    print(f"{func}: target {len(tw)} words, candidate {len(gw)} words, "
          f"{diff} differing ({'masked' if masked else 'raw'})")
    print(f"{'':4s} {'candidate':>9s} {'target':>9s}  "
          f"{'candidate text':38s} target text")
    for i in range(max(len(gw), len(tw))):
        g = gw[i] if i < len(gw) else None
        t = tw[i] if i < len(tw) else None
        same = ((gc[i] if i < len(gc) else None)
                == (tc[i] if i < len(tc) else None))
        gt = got[i][2] if i < len(got) else ""
        tt = tgt[i][1] if i < len(tgt) else ""
        print(f"{i:4d} {('%08X' % g) if g is not None else '--------':>9s} "
              f"{('%08X' % t) if t is not None else '--------':>9s}  "
              f"{gt:38s} {tt}" + ("" if same else "   <<<"))
    return diff


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("src", nargs="?")
    ap.add_argument("funcs", nargs="*")
    ap.add_argument("--dump", metavar="FUNC",
                    help="print one target function and exit")
    ap.add_argument("--G", type=int, default=0,
                    help="cc1psx -G (default 0, which is what the overlays are)")
    ap.add_argument("--raw", action="store_true",
                    help="compare raw words, without masking unresolved fields")
    args = ap.parse_args()

    if args.dump:
        for i, (w, t) in enumerate(target(args.dump)):
            print(f"{i:4d} {w:08X}  {t}")
        return

    if not args.src or not args.funcs:
        ap.error("need a candidate .c and at least one function name")

    words, relocs = asmlib.build(args.src, gasG=0, cflags=(f"-G{args.G}",))
    # Recover each function's extent from the object's symbol table: objdump
    # gives one flat .text, so the candidate has to be cut at the symbols.
    o = os.path.join(asmlib.SP,
                     os.path.splitext(os.path.basename(args.src))[0] + ".o")
    syms = {}
    for ln in subprocess.run(["mips64-linux-gnu-nm", "-n", o],
                             capture_output=True, text=True).stdout.split("\n"):
        m = re.match(r"([0-9a-f]+) [Tt] (\S+)", ln)
        if m:
            syms[m.group(2)] = int(m.group(1), 16)
    total = 0
    for f in args.funcs:
        if f not in syms:
            print(f"{f}: not defined in {args.src}")
            total += 1
            continue
        lo = syms[f]
        later = sorted(v for v in syms.values() if v > lo)
        hi = later[0] if later else max(a for a, _, _ in words) + 4
        body = [(a, w, t) for a, w, t in words if lo <= a < hi]
        total += report(f, body, relocs, target(f), not args.raw)
        print()
    sys.exit(1 if total else 0)


if __name__ == "__main__":
    main()
