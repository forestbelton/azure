#!/usr/bin/env python3
"""nearsibs.py UNIT -- find stub pairs that are the same source with small edits.

`families.py` groups by instruction shape with `jal` targets and i-type
immediates masked, so it finds members that differ only in constants. It
CANNOT see a pair that differs in an operator, a branch condition, a register
allocation or a systematic field-offset swap -- docs/town-sweep.md's "A
family's NEIGHBOURS are often near-siblings it does not group" is about exactly
that, and until now finding them was a manual diff.

This compares the MNEMONIC STREAM of every stub against every other stub of
similar length and reports the pairs above a similarity threshold. It found the
four-function collision sweep at 0x8008DA80 (1,334 words, landed from one
reading) and func_800BA930/func_800BACB0 (448 words) in one run.

    python3 tools/decomp/nearsibs.py ovl_town
    python3 tools/decomp/nearsibs.py ovl_town --min-words 40 --ratio 0.92
    python3 tools/decomp/nearsibs.py ovl_town --groups      # transitive sets

A high ratio is a strong lead, not a promise: 1.000 means the two have the
same mnemonics in the same order, which is what a twin looks like, but also
what two functions built from one macro look like. Read the dumps before
promising a word count.

Parked functions are excluded, the same as worklist.py does.
"""
import argparse
import difflib
import io
import os
import re
import sys
from collections import defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.environ.get("AZURE_ROOT", os.path.dirname(os.path.dirname(HERE)))


def parked():
    d = os.path.join(PROJ, "docs/near-misses")
    if not os.path.isdir(d):
        return set()
    return {f[:-2] for f in os.listdir(d)
            if f.startswith("func_") and f.endswith(".c")}


def stubs_of(unit):
    """The INCLUDE_ASM stubs of UNIT, from whichever csrc file carries them."""
    import glob
    pat = re.compile(r'INCLUDE_ASM\("asm/nonmatchings/%s", (func_[0-9A-Fa-f]{8})\)'
                     % re.escape(unit))
    found = set()
    for p in glob.glob(os.path.join(PROJ, "csrc", "*.c")):
        found |= set(pat.findall(io.open(p, encoding="utf-8", errors="replace").read()))
    return found


def load(unit, names, min_words):
    """size and mnemonic stream per stub, read from its splat listing."""
    root = os.path.join(PROJ, "asm/nonmatchings", unit)
    sizes, streams = {}, {}
    for f in sorted(names):
        p = os.path.join(root, f + ".s")
        if not os.path.exists(p):
            continue
        t = io.open(p, encoding="utf-8", errors="replace").read()
        m = re.search(r"nonmatching %s, 0x([0-9A-Fa-f]+)" % f, t)
        if not m:
            continue
        n = int(m.group(1), 16) // 4
        if n < min_words:
            continue
        sizes[f] = n
        streams[f] = re.findall(r"/\* \w+ \w+ \w{8} \*/\s+(\S+)", t)
    return sizes, streams


def pairs_of(sizes, streams, ratio, bucket=8):
    """Compare only within adjacent length buckets; O(n^2) over 1000 stubs is
    minutes, this is seconds."""
    buckets = defaultdict(list)
    for f, n in sizes.items():
        buckets[n // bucket].append(f)
    seen, out = set(), []
    for k in sorted(buckets):
        cand = buckets[k] + buckets.get(k + 1, [])
        for i, a in enumerate(cand):
            for b in cand[i + 1:]:
                if a == b or (a, b) in seen or (b, a) in seen:
                    continue
                seen.add((a, b))
                sm = difflib.SequenceMatcher(None, streams[a], streams[b])
                if sm.quick_ratio() < ratio:
                    continue
                r = sm.ratio()
                if r >= ratio:
                    out.append((r, a, b))
    return sorted(out, key=lambda t: (-t[0], t[1]))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("unit")
    ap.add_argument("--min-words", type=int, default=40,
                    help="skip stubs smaller than this; families.py covers them")
    ap.add_argument("--ratio", type=float, default=0.92)
    ap.add_argument("--groups", action="store_true",
                    help="merge pairs into transitive sets")
    a = ap.parse_args()

    skip = parked()
    names = stubs_of(a.unit) - skip
    if not names:
        sys.exit("nearsibs.py: no stubs found for unit %r" % a.unit)
    sizes, streams = load(a.unit, names, a.min_words)
    if skip:
        print("parked, not listed: %s" % " ".join(sorted(skip & stubs_of(a.unit))))
    ps = pairs_of(sizes, streams, a.ratio)

    if a.groups:
        parent = {f: f for f in sizes}

        def find(x):
            while parent[x] != x:
                parent[x] = parent[parent[x]]
                x = parent[x]
            return x

        for _, x, y in ps:
            parent[find(x)] = find(y)
        groups = defaultdict(list)
        for f in sizes:
            groups[find(f)].append(f)
        sets = sorted((g for g in groups.values() if len(g) > 1),
                      key=lambda g: -sum(sizes[f] for f in g))
        tot = 0
        for g in sets:
            w = sum(sizes[f] for f in g)
            tot += w
            print("  %4d words  %d members  %s"
                  % (w, len(g), " ".join("%s(%d)" % (f, sizes[f]) for f in sorted(g))))
        print("\n%d sets, %d words -- one reading each if they really are siblings"
              % (len(sets), tot))
        return

    tot = 0
    for r, x, y in ps:
        tot += sizes[x] + sizes[y]
        print("  %.3f  %-16s %4dw  ~  %-16s %4dw   pair=%d"
              % (r, x, sizes[x], y, sizes[y], sizes[x] + sizes[y]))
    print("\n%d pairs at ratio >= %.2f, %d words"
          % (len(ps), a.ratio, tot))


if __name__ == "__main__":
    main()
