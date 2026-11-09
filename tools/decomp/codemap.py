#!/usr/bin/env python3
"""Find the executable code in a container WITHOUT being told where it loads.

The three overlays mapped before this existed were each located by voting a
block's own `jal` targets against its `addiu $sp,$sp,-N` prologues and taking
the offset-to-address delta with the most agreement (see the notes in
splat/*.yaml). That test answers "does THIS delta beat the others for a span I
already suspect"; it is not a search, and used as one it is far too blunt --
run over a whole container it scores 0-45 for real code and 0-33 for noise,
which is how dungeon.bin came to be documented as holding exactly one code
overlay when it holds about two hundred.

This tool replaces the prologue vote with a FUNCTION-START test. cc1psx emits
functions back to back, so the two words before a function are `jr $ra` and
its delay slot. A `jal` inside a linked object that targets that same object
must therefore land exactly two words after an 0x03E00008. Scored over a
window, the correct delta takes essentially every internal target and a wrong
one takes almost none: on the known dungeon overlay it is 453 of 453 (100%)
against 4% for the runner-up, where the prologue vote managed 1484 against 374.
That margin is what makes a blind sweep possible.

It assumes nothing about the load address beyond "it is in RAM" -- the retail
2 MB plus the 8 MB devkit window main.bin's dead 0x80400000 twin needs -- and
nothing about alignment, size or layout. Prologues are used only as a cheap
prefilter and as a corroborating count.

Usage:
    python3 tools/decomp/codemap.py data/dungeon/dungeon.bin
    python3 tools/decomp/codemap.py data/dungeon/dungeon.bin --window 0x6000
    python3 tools/decomp/codemap.py data/dungeon/dungeon.bin --pin 0xa3d40 0xf7700
    python3 tools/decomp/codemap.py data/dungeon/dungeon.bin --distinct

--pin reports the ranked deltas for one range you already suspect, which is
what to use when checking a boundary by hand. --distinct additionally collapses
the hits that are the same object relinked at another address, because
dungeon.bin stores most of its overlays as seven copies -- one per RAM slot --
and only one copy of each is work.

A hit is evidence, not a segment boundary. The window is sector-granular, so
read a hit as "code is in here" and then pin the exact ends with --pin, the
way splat/dungeon.bin.yaml's own text-end note does it.
"""
import argparse
import collections
import hashlib
import os
import sys

import numpy as np

JRRA = 0x03E00008
PRO = 0x27BD0000
SECTOR = 0x800

# Retail RAM is 0x80000000..0x801FFFFF. 0x80400000..0x807FFFFF is upper RAM on
# an 8 MB DTL-H2000 board, where main.bin's assert-enabled debug twin lives; it
# is dead on the shipped disc but it is still code in a container.
RAM = ((0x80010000, 0x80200000), (0x80400000, 0x80800000))


def in_ram(v):
    return any(lo <= v < hi for lo, hi in RAM)


class Image:
    def __init__(self, path):
        self.path = path
        self.w = np.fromfile(path, dtype="<u4")
        self.nbytes = len(self.w) * 4
        op = self.w >> 26
        self.is_jal = op == 0x03
        self.tgt = np.where(
            self.is_jal, 0x80000000 | ((self.w & 0x03FFFFFF) << 2), 0
        ).astype(np.int64)
        self.is_jrra = self.w == JRRA
        self.is_pro = ((self.w & 0xFFFF0000) == PRO) & ((self.w & 0x8000) != 0)
        self.jal_idx = np.flatnonzero(self.is_jal)
        self.jrra_idx = np.flatnonzero(self.is_jrra)
        self.csum_pro = np.concatenate(([0], np.cumsum(self.is_pro)))
        self.csum_rra = np.concatenate(([0], np.cumsum(self.is_jrra)))

    def span(self, c, a, b):
        return int(c[b] - c[a])

    def pin(self, lo, hi, mint=10, topn=1):
        """Rank load deltas for [lo, hi) in bytes. Returns [(score, ntgt, npro, delta)]."""
        a, b = lo >> 2, hi >> 2
        ji = self.jal_idx[
            np.searchsorted(self.jal_idx, a) : np.searchsorted(self.jal_idx, b)
        ]
        ri = self.jrra_idx[
            np.searchsorted(self.jrra_idx, a) : np.searchsorted(self.jrra_idx, b)
        ]
        if len(ji) < mint or len(ri) < 4:
            return []
        tg = np.unique(self.tgt[ji])
        # Every delta that would put some internal target on some function
        # start. `jr $ra` index + 2 words is the next function's first word.
        starts = (ri + 2) * 4
        vals, cnts = np.unique((tg[:, None] - starts[None, :]).ravel(), return_counts=True)
        vals = vals[cnts >= 3]
        out = []
        for d in vals.tolist():
            if not in_ram((lo + d) & 0xFFFFFFFF):
                continue
            inr = tg[(tg - d >= lo) & (tg - d < hi)]
            if len(inr) < mint:
                continue
            idx = ((inr - d) >> 2) - 2
            score = int(np.count_nonzero(self.is_jrra[idx]))
            npro = int(np.count_nonzero(self.is_pro[((inr - d) >> 2)]))
            out.append((score, len(inr), npro, d))
        out.sort(reverse=True)
        return out[:topn]

    def sweep(self, window, mint, thresh):
        """Pin a window at every sector offset; keep the sharp hits."""
        hits = []
        for lo in range(0, self.nbytes - window + 1, SECTOR):
            a, b = lo >> 2, (lo + window) >> 2
            if self.span(self.csum_pro, a, b) < 8 or self.span(self.csum_rra, a, b) < 8:
                continue
            got = self.pin(lo, lo + window, mint=mint)
            if not got:
                continue
            score, ntgt, npro, d = got[0]
            if score >= thresh * ntgt:
                hits.append((lo, d, score, ntgt, npro))
        return hits

    def fingerprint(self, lo, size):
        """Relink-invariant hash: two copies of one object linked at different
        addresses hash the same. Immediates, jump targets and relocated pointer
        words are masked out; opcode and register fields are kept."""
        w = self.w[lo >> 2 : (lo + size) >> 2].astype(np.uint32).copy()
        op = w >> 26
        # A word holding a RAM address is a relocated pointer, not an insn:
        # `lb rt, imm($zero)` is not something cc1psx or gas emit.
        ptr = (w >= 0x80000000) & (w < 0x80200000)
        w = np.where(ptr, np.uint32(0x80000000), w)
        w = np.where(~ptr & ((op == 0x02) | (op == 0x03)), w & np.uint32(0xFC000000), w)
        imm = ~ptr & (
            ((op >= 0x04) & (op <= 0x0F))
            | ((op >= 0x20) & (op <= 0x2E))
            | (op == 0x32)
            | (op == 0x3A)
            | (op == 0x01)
        )
        w = np.where(imm, w & np.uint32(0xFFFF0000), w)
        return hashlib.sha1(w.tobytes()).hexdigest()


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("image")
    ap.add_argument("--window", type=lambda s: int(s, 0), default=0x6000,
                    help="sweep window in bytes (default 0x6000, dungeon.bin's bank size)")
    ap.add_argument("--min-targets", type=int, default=10,
                    help="distinct internal jal targets a window must have (default 10)")
    ap.add_argument("--threshold", type=float, default=0.90,
                    help="fraction of them that must sit after a jr $ra (default 0.90)")
    ap.add_argument("--pin", nargs=2, metavar=("LO", "HI"),
                    help="rank deltas for one range instead of sweeping")
    ap.add_argument("--distinct", action="store_true",
                    help="also collapse hits that are one object relinked")
    args = ap.parse_args()

    img = Image(args.image)
    print("%s  %d bytes (0x%x)" % (args.image, img.nbytes, img.nbytes))

    if args.pin:
        lo, hi = int(args.pin[0], 0), int(args.pin[1], 0)
        got = img.pin(lo, hi, mint=1, topn=6)
        print("pinning 0x%x..0x%x (%d bytes, %d words)" % (lo, hi, hi - lo, (hi - lo) // 4))
        if not got:
            print("  no delta puts an internal jal target on a function start")
            return 0
        for score, ntgt, npro, d in got:
            print("  vram 0x%08x..0x%08x  delta 0x%08x | %4d/%-4d internal targets "
                  "after a jr $ra (%3.0f%%), %4d on a prologue"
                  % ((lo + d) & 0xFFFFFFFF, (hi + d) & 0xFFFFFFFF, d & 0xFFFFFFFF,
                     score, ntgt, 100.0 * score / ntgt, npro))
        return 0

    hits = img.sweep(args.window, args.min_targets, args.threshold)
    print("window 0x%x, >=%d internal targets, >=%.0f%% after a jr $ra: %d sector offsets hit"
          % (args.window, args.min_targets, args.threshold * 100, len(hits)))

    runs = []
    for lo, d, score, ntgt, npro in hits:
        if runs and lo == runs[-1][1] + SECTOR and d == runs[-1][2]:
            runs[-1][1] = lo
            runs[-1][3] = max(runs[-1][3], ntgt)
        else:
            runs.append([lo, lo, d, ntgt])
    print("%d runs of constant delta -- each is one linked object" % len(runs))
    print()
    print("%-21s %-11s %-23s %s" % ("file (first..last hit)", "sectors", "load address",
                                    "peak targets"))
    for lo, hi, d, ntgt in runs:
        print("0x%07x..0x%07x  %-11d 0x%08x..0x%08x  %d"
              % (lo, hi, (hi - lo) // SECTOR + 1, (lo + d) & 0xFFFFFFFF,
                 (hi + args.window + d) & 0xFFFFFFFF, ntgt))

    print()
    band = collections.Counter()
    for lo, hi, d, _ in runs:
        band[((lo + d) & 0xFFFFFFFF) >> 16] += 1
    print("load-address bands:")
    for b, c in sorted(band.items()):
        rs = [r for r in runs if (((r[0] + r[2]) & 0xFFFFFFFF) >> 16) == b]
        print("  0x%04x????  %3d objects   file 0x%07x..0x%07x"
              % (b, c, min(r[0] for r in rs), max(r[1] for r in rs) + args.window))

    if args.distinct:
        fps = collections.defaultdict(list)
        for lo, hi, d, _ in runs:
            if lo + args.window <= img.nbytes:
                fps[img.fingerprint(lo, args.window)].append(lo)
        print()
        print("distinct objects (relink-invariant): %d of %d hits" % (len(fps), len(runs)))
        mult = collections.Counter(len(v) for v in fps.values())
        print("copies per object:", sorted(mult.items()))

    return 0


if __name__ == "__main__":
    sys.exit(main())
