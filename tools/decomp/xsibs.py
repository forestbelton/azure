#!/usr/bin/env python3
"""xsibs.py UNIT -- near-siblings of a unit's stubs among functions already
LANDED ANYWHERE IN THE PROJECT, not just in that unit.

`families.py` and `nearsibs.py` both compare a unit's stubs against each
other, and `landedshapes.py` compares them against landed functions OF THE
SAME UNIT -- which reports nothing once splat has pruned those listings. None
of the three can answer "has this already been written in another container",
and that is the first question to ask of a block that has just been opened:
if the answer is yes in bulk, the block is a renaming job rather than a
decompilation.

The landed side is read out of the REFERENCE IMAGES through each container's
linked ELF (symbol table for the boundaries, section LMA for the file offset),
so it does not depend on a listing surviving. The comparison is on the opcode
stream -- registers, immediates and jump targets masked -- which is what
survives a relink and a different record layout.

    python3 tools/decomp/xsibs.py ovl_dungeon
    python3 tools/decomp/xsibs.py ovl_dungeon --min-words 24 --ratio 0.92

MEASURED ON ovl_dungeon, 2026-11-14, the day it was opened: 19 of its 863
functions have a landed near-sibling at ratio >= 0.88, totalling 550 words --
0.6% of the block. dungeon.bin's overlay and town.bin's occupy the same RAM
slot and share almost no source. Read a result like that as a negative worth
having: it says the block has to be read rather than renamed.

A high ratio is a lead, not a promise. func_800BCE7C/func_8008CE08 came back
at 1.000 and is a verbatim landing with every field displacement 0xE8 higher;
the 0.9xx rows are a starting body, not an answer.
"""
import argparse
import bisect
import collections
import difflib
import glob
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.environ.get("AZURE_ROOT", os.path.dirname(os.path.dirname(HERE)))
WORD = re.compile(r"^\s*/\* [0-9A-Fa-f]+ [0-9A-Fa-f]{8} ([0-9A-Fa-f]{8}) \*/")

# Every container that links landed C, with the reference image it is checked
# against. A container with no landed code contributes nothing and is skipped.
CONTAINERS = [
    ("build/town.bin.elf", "data/town/town.bin"),
    ("build/main.bin.elf", "data/main/main.bin"),
    ("build/slus_006.14.elf", "data/slus_006.14"),
    ("build/dungeon.bin.elf", "data/dungeon/dungeon.bin"),
]

PREFIX = os.environ.get("MIPS_PREFIX")
if not PREFIX:
    PREFIX = ("mipsel-linux-gnu-"
              if subprocess.run(["which", "mipsel-linux-gnu-nm"],
                                capture_output=True).returncode == 0
              else "mips64-linux-gnu-")


def ops(words):
    """Opcode stream: the major opcode, or the funct field for an R-type.

    Registers, immediates and jump targets are all dropped, which is what
    makes a relinked or re-laid-out copy of one source compare equal.
    """
    out = []
    for v in words:
        op = v >> 26
        out.append(0x40 + (v & 0x3F) if op == 0 else op)
    return out


def stub_streams(unit, minw):
    out = {}
    d = os.path.join(PROJ, "asm/nonmatchings", unit)
    if not os.path.isdir(d):
        sys.exit("no listings for unit %s" % unit)
    for p in sorted(glob.glob(os.path.join(d, "func_*.s"))):
        w = []
        for line in open(p):
            m = WORD.match(line)
            if m:
                w.append(int.from_bytes(bytes.fromhex(m.group(1)), "little"))
        if len(w) >= minw:
            out[os.path.basename(p)[:-2]] = ops(w)
    return out


def landed_streams(minw, skip_unit_listings):
    """Every named function in every container, taken from the reference image.

    A function's end is the next symbol of ANY kind, which keeps a migrated
    rodata label from being absorbed; `D_`-named and compiler-emitted symbols
    are boundaries but not candidates.
    """
    out = {}
    for elf, img in CONTAINERS:
        ep, ip = os.path.join(PROJ, elf), os.path.join(PROJ, img)
        if not (os.path.exists(ep) and os.path.exists(ip)):
            continue
        secs = []
        for line in subprocess.run([PREFIX + "objdump", "-h", ep],
                                   capture_output=True, text=True).stdout.splitlines():
            f = line.split()
            if len(f) >= 6 and f[0].isdigit():
                secs.append((int(f[3], 16), int(f[2], 16), int(f[4], 16)))  # vma,size,lma
        data = open(ip, "rb").read()
        allsym, cand = [], []
        for line in subprocess.run([PREFIX + "nm", "-n", ep],
                                   capture_output=True, text=True).stdout.splitlines():
            f = line.split()
            if len(f) == 3 and f[1] in "tT":
                a = int(f[0], 16)
                allsym.append(a)
                # `.NON_MATCHING` is the suffix splat's INCLUDE_ASM gives a
                # stub's own glabel. Those are the assembly we are trying to
                # replace, not landed C, and leaving them in makes every stub
                # match itself at 1.000.
                # An ALL-CAPS name is one of splat's generated linker-script
                # boundary symbols (A3000_TEXT_START and friends). They sit at
                # a real function's address and would be reported as a 1.000
                # match for it.
                if (not f[2].startswith(("D_", "gcc2", "__gnu", "jtbl_"))
                        and ".NON_MATCHING" not in f[2]
                        and not re.fullmatch(r"[A-Z0-9_]+", f[2])):
                    cand.append((a, f[2]))
        allsym = sorted(set(allsym))
        for a, n in cand:
            i = bisect.bisect_right(allsym, a)
            nw = ((allsym[i] if i < len(allsym) else a) - a) // 4
            if nw < minw or nw > 4000:
                continue
            off = None
            for vma, size, lma in secs:
                if vma <= a < vma + size:
                    off = lma + (a - vma)
                    break
            if off is None or off + 4 * nw > len(data):
                continue
            w = [int.from_bytes(data[off + 4 * k: off + 4 * k + 4], "little")
                 for k in range(nw)]
            out["%s:%s" % (os.path.basename(img), n)] = ops(w)
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("unit")
    ap.add_argument("--min-words", type=int, default=16,
                    help="skip stubs smaller than this; families.py covers them")
    ap.add_argument("--ratio", type=float, default=0.88)
    ap.add_argument("--top", type=int, default=60)
    a = ap.parse_args()

    stubs = stub_streams(a.unit, a.min_words)
    landed = landed_streams(a.min_words, a.unit)
    # a stub of this unit can still reach `landed` under its own container if
    # it is a stub that splat did NOT suffix; drop those self-matches, which
    # are trivially 1.000 and say nothing
    own = set(stubs)
    landed = {k: v for k, v in landed.items() if k.split(":", 1)[1] not in own}
    print("%s: %d stubs >= %dw, %d landed functions to compare against"
          % (a.unit, len(stubs), a.min_words, len(landed)), file=sys.stderr)

    bylen = collections.defaultdict(list)
    for n, o in landed.items():
        bylen[len(o)].append((n, o))
    mset = {n: collections.Counter(o) for n, o in landed.items()}

    rows = []
    for dn, do in stubs.items():
        L = len(do)
        dm = collections.Counter(do)
        best = []
        for ln in range(int(L * a.ratio), int(L / a.ratio) + 2):
            for n, o in bylen.get(ln, ()):
                # multiset overlap bounds SequenceMatcher's match count, so
                # this prefilter is exact: it can only skip pairs that cannot
                # reach the ratio
                if sum((dm & mset[n]).values()) < a.ratio * (L + ln) / 2:
                    continue
                r = difflib.SequenceMatcher(None, do, o, autojunk=False).ratio()
                if r >= a.ratio:
                    best.append((r, n))
        if best:
            best.sort(reverse=True)
            rows.append((L, dn, best[:3]))

    rows.sort(key=lambda r: -r[0])
    print("\n%d stub(s) with a landed near-sibling at ratio >= %.2f, %d words\n"
          % (len(rows), a.ratio, sum(r[0] for r in rows)))
    for L, dn, best in rows[:a.top]:
        print("%5d  %-16s  %s" % (L, dn, "  ".join("%.3f %s" % (r, n) for r, n in best)))


main()
