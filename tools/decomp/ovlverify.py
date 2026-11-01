#!/usr/bin/env python3
"""verify.py's job for an OVERLAY function, whose asm is one big `asm` segment.

`tools/decomp/verify.py` reads its target from `asm/nonmatchings/<unit>/<func>.s`,
which splat only emits for a `c` subsegment. The overlays are `asm`
subsegments in one file each, so there is nothing there to read and the target
has to come out of the single listing.

The overlays are -G0, NOT the -G8 the resident executable is built with, and
that is not a style choice: at -G8 cc1psx records `.extern SYM,4` and the
access comes out as the assembler-expanded `lui $at` / `%lo` pair, while at -G0
cc1psx emits its own `%hi`/`%lo` split address into a general register -- the
hoisted-lui shape the live overlay uses. Measured on func_80020924: -G8 gives
26 words with two `lui $at`, -G0 gives 25 with `lui $v1`, and the target is 24.
(The debug twin at 0x80400000 uses the `lui $at` shape throughout, so IT looks
like a -G8 build of the same source. Do not assume one -G fits both blocks.)

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
    ("ovl_town", 0x8008AD90, 0x800CF720),
    ("ovl_dungeon", 0x80089588, 0x800DCE60),
]
ROW = re.compile(r"\s*/\* \w+ (\w+) ([0-9A-F]{8}) \*/\s+(.*)")


def overlay_for(func):
    a = int(func.split("_")[1], 16)
    for name, lo, hi in OVERLAYS:
        if lo <= a < hi:
            return name
    raise SystemExit(f"{func} is in none of the split overlays")


def target(func):
    """[(word, text)] for one function, out of the overlay's single listing."""
    path = os.path.join(PROJ, "asm", overlay_for(func) + ".s")
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
        raise SystemExit(f"{func} not found in asm/{overlay_for(func)}.s")
    return out


def norm(w, text):
    """Mask fields a candidate object cannot have filled in yet.

    An unresolved `jal` or `%hi`/`%lo` in a freshly assembled object reads 0,
    so comparing them raw reports every global access as wrong. Masking them
    is what makes the remaining diff mean something -- it is the same choice
    advise.py makes when it reports an aligned verdict instead of a positional
    one.
    """
    op = w >> 26
    if op in (2, 3):
        return 0x0C000000 if op == 3 else 0x08000000
    if op == 0x0F:                      # lui: drop imm, keep dest
        return w & 0xFFFF0000
    if op in (0x23, 0x2B, 0x21, 0x25, 0x29, 0x28, 0x20, 0x24, 0x09, 0x0D):
        return w & 0xFFFF0000           # load/store/addiu/ori: drop imm
    return w


def report(func, got, tgt, masked):
    gw = [w for _, w, _ in got]
    tw = [w for w, _ in tgt]
    if masked:
        gc = [norm(w, t) for _, w, t in got]
        tc = [norm(w, t) for w, t in tgt]
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
        total += report(f, body, target(f), not args.raw)
        print()
    sys.exit(1 if total else 0)


if __name__ == "__main__":
    main()
