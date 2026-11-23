#!/usr/bin/env python3
"""Every code object in DUNGEON.BIN, enumerated from the loaders that read it.

`codemap.py` finds code in a container without being told where it loads. That
is what opened this container, but it is a SEARCH: its window is sector
granular, so a family stored as seven relinked copies smears across deltas and
the object count comes out as a bound ("at most 118") rather than a count.

This tool does not search. The dungeon overlay is decompiled, so the two
functions that read code out of this container are readable C, and both
compute their LBA arithmetically:

    func_800A0B94  (csrc/ovl_dungeon_8009F644.c)   the per-slot banks
        nsec 0xC, dest 0x80170800 - i * 0x6000,
        LBA  (a - 1) * 84 + (i * 12 + 0x4340)
        plus two fixed resources: a == 0x31 -> 0x18 sectors at 0x8016A800,
        LBA 0x569C, and a == 0x38 -> 0x24 sectors at 0x80164800, LBA 0x56B1.

    func_800A982C  (csrc/ovl_dungeon_800A4B88.c)   the 0x80024000 objects
        nsec 0xC, dest 0x80024000, LBA (a - 1) * 12 + 0x607F

dungeon.bin starts at LBA 12310: the overlay load table at 0x80080C40 reads
LBA 12636 for dungeon.bin+0xa3000 (docs/town-map.md section 1). Subtracting it
turns both formulas into file offsets, and every one of them can then be
checked rather than guessed -- `--verify` pins each predicted bank with
codemap's function-start test and prints the score.

    python3 tools/decomp/dungeon_objects.py
    python3 tools/decomp/dungeon_objects.py --verify
    python3 tools/decomp/dungeon_objects.py --list

A grid slot is only an object when the image agrees. Banks past the end of a
family hold texture records, which is what the `a` range printed by `--list`
reports; the test for membership is the head word, which every object in both
families starts with -- a pointer to the object's own entry function, in the
object's own load window, at a word that is `addiu $sp, $sp, -N`.
"""
import argparse
import collections
import hashlib
import os
import sys

import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
from codemap import Image  # noqa: E402

PROJ = os.environ.get("AZURE_ROOT", os.path.dirname(os.path.dirname(HERE)))
IMG = os.path.join(PROJ, "data/dungeon/dungeon.bin")

BASE_LBA = 12310
SECTOR = 0x800
JRRA = 0x03E00008
BANK = 0x6000

# The seven RAM slots func_800A0B94 descends through, and the window every
# bank's internal references fall in.
SLOT0 = 0x80170800
SLOT_LO, SLOT_HI = SLOT0 - 6 * BANK, SLOT0 + BANK
OVL24 = 0x80024000


def off_of(lba):
    return (lba - BASE_LBA) * SECTOR


def is_pro(w):
    return (w & 0xFFFF0000) == 0x27BD0000 and (w & 0x8000)


class Obj:
    """One self-contained relinked object: a header of pointers, then text."""

    def __init__(self, img, off, size, vram):
        self.off, self.size, self.vram = off, size, vram
        self.w = img.w[off >> 2: (off + size) >> 2]
        self.head = int(self.w[0])
        self.fns = []
        self.start = self.end = 0

    def entry_ok(self):
        if not (self.vram <= self.head < self.vram + self.size):
            return False
        return bool(is_pro(int(self.w[(self.head - self.vram) >> 2])))

    def split(self):
        """Function starts: every `jr $ra` + 2, every header pointer that aims
        at a prologue, and every internal `jal` target. The entry pointer alone
        is not enough -- 13 of the 47 banks have text BELOW it."""
        b = self.w
        jr = [int(i) for i in np.flatnonzero(b == JRRA)]
        if not jr:
            return False
        end = jr[-1] + 2
        s = set(i + 2 for i in jr if i + 2 < end)
        for k in range(end):
            v = int(b[k])
            if self.vram <= v < self.vram + self.size:
                t = (v - self.vram) >> 2
                if t < end and is_pro(int(b[t])):
                    s.add(t)
        op = b[:end] >> 26
        for k in np.flatnonzero(op == 3).tolist():
            t = int(0x80000000 | ((int(b[k]) & 0x03FFFFFF) << 2))
            if self.vram <= t < self.vram + self.size:
                s.add((t - self.vram) >> 2)
        st = sorted(x for x in s if 0 < x < end)
        while st and not is_pro(int(b[st[0]])):
            st.pop(0)
        if not st:
            return False
        self.start, self.end = st[0] * 4, end * 4
        self.fns = [(st[k] * 4, (st[k + 1] if k + 1 < len(st) else end) - st[k])
                    for k in range(len(st))]
        return True

    def words(self):
        return sum(n for _, n in self.fns)

    def fnhash(self, off, n, lo, hi):
        """Relink-invariant: zero the words the relink rewrites -- pointers
        into the object's own load window and `jal`/`j` fields that target it
        -- and keep everything else, external calls included."""
        w = self.w[off >> 2: (off >> 2) + n].astype(np.uint32).copy()
        op = w >> 26
        ptr = (w >= lo) & (w < hi)
        w = np.where(ptr, np.uint32(0), w)
        tgt = 0x80000000 | ((w & 0x03FFFFFF) << 2)
        jin = (~ptr) & ((op == 2) | (op == 3)) & (tgt >= lo) & (tgt < hi)
        w = np.where(jin, w & np.uint32(0xFC000000), w)
        return hashlib.sha1(w.tobytes()).hexdigest()


def banked(img):
    """func_800A0B94's grid: 7 relinked copies of each object, one per slot."""
    objs = {}
    for a in range(1, 80):
        off = off_of(0x4340 + (a - 1) * 84)
        if off < 0 or off + BANK > img.nbytes:
            break
        o = Obj(img, off, BANK, SLOT0)
        if o.entry_ok() and o.split():
            objs[a] = o
    return objs


def ovl24(img):
    """func_800A982C's grid: one copy each, all at 0x80024000."""
    objs = {}
    for a in range(1, 110):
        off = off_of(0x607F + (a - 1) * 12)
        if off < 0 or off + BANK > img.nbytes:
            break
        o = Obj(img, off, BANK, OVL24)
        if o.entry_ok() and o.split():
            objs[a] = o
    return objs


def fixed(img):
    """The objects whose LBA is a constant rather than a grid position."""
    out = {}
    o = Obj(img, off_of(0x569C), 0xC000, SLOT0 - BANK)   # func_800A0B94, a == 0x31
    if o.entry_ok() and o.split():
        out["a=0x31"] = o
    for nm, off, size, vram in (("0x80016000", 0x2A3000, 0xD800, 0x80016000),
                                ("0x80020000", 0x182B000, 0x9800, 0x80020000)):
        o = Obj(img, off, size, vram)
        if o.split():
            out[nm] = o
    return out


def tally(objs, lo, hi, copies=1):
    h = collections.defaultdict(list)
    for a, o in objs.items():
        for off, n in o.fns:
            h[o.fnhash(off, n, lo, hi)].append((a, off, n))
    nf = sum(len(o.fns) for o in objs.values())
    nw = sum(o.words() for o in objs.values())
    uw = sum(v[0][2] for v in h.values())
    return nf, nw, len(h), uw, h


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("image", nargs="?", default=IMG)
    ap.add_argument("--list", action="store_true", help="one row per object")
    ap.add_argument("--verify", action="store_true",
                    help="pin every predicted bank with the function-start test")
    args = ap.parse_args()

    img = Image(args.image)
    A, B, F = banked(img), ovl24(img), fixed(img)

    print("%s  %d bytes" % (args.image, img.nbytes))
    print()
    nfa, nwa, dfa, dwa, ha = tally(A, SLOT_LO, SLOT_HI)
    nfb, nwb, dfb, dwb, hb = tally(B, OVL24, OVL24 + BANK)
    print("func_800A0B94's banks   a=1..%-3d  %2d objects x 7 slot copies  "
          "%4d fns  %6d words" % (max(A), len(A), nfa, nwa))
    print("    distinct, relink-invariant: %4d fns  %6d words "
          "(1.%02dx within the family)" % (dfa, dwa, round(100 * nwa / dwa) - 100))
    print("func_800A982C's objects a=1..%-3d  %2d objects, one copy each   "
          "%4d fns  %6d words" % (max(B), len(B), nfb, nwb))
    print("    distinct, relink-invariant: %4d fns  %6d words" % (dfb, dwb))
    print("shared between the two families: %d functions"
          % len(set(ha) & set(hb)))
    print()
    for nm, o in F.items():
        print("%-12s file 0x%07x  vram 0x%08x  text 0x%04x..0x%04x  "
              "%3d fns  %5d words" % (nm, o.off, o.vram, o.start, o.end,
                                      len(o.fns), o.words()))
    print()
    dist = dwa + dwb + sum(o.words() for o in F.values())
    # on the disc: every bank seven times, and 0x80016000 twice -- the copy at
    # +0x51e800 is byte-identical to +0x2a3000 over a 0x30000 run.
    stored = (7 * nwa + nwb + sum(o.words() for o in F.values())
              + F["0x80016000"].words())
    print("distinct code to decompile: %d words" % dist)
    print("as stored on the disc:      %d words (%.1fx)" % (stored, stored / dist))

    if args.list:
        print()
        print("%-6s %-10s %-11s %-18s %4s %6s" %
              ("a", "file", "vram", "text", "fns", "words"))
        for nm, objs, lo, hi in (("bank", A, SLOT_LO, SLOT_HI),
                                 ("ovl24", B, OVL24, OVL24 + BANK)):
            for a in sorted(objs):
                o = objs[a]
                print("%-6s 0x%07x  0x%08x  0x%04x..0x%04x  %4d %6d" %
                      ("%s %d" % (nm, a), o.off, o.vram, o.start, o.end,
                       len(o.fns), o.words()))

    if args.verify:
        print()
        bad = 0
        for a in sorted(A):
            for i in range(7):
                off = off_of(0x4340 + (a - 1) * 84 + i * 12)
                vram = SLOT0 - i * BANK
                got = img.pin(off, off + BANK, mint=1, topn=1)
                ok = got and ((off + got[0][3]) & 0xFFFFFFFF) == vram
                if not ok:
                    bad += 1
                    print("  bank a=%d i=%d (0x%07x): expected 0x%08x, got %s"
                          % (a, i, off, vram,
                             "0x%08x" % ((off + got[0][3]) & 0xFFFFFFFF)
                             if got else "no delta"))
        print("  %d of %d banks pin at the slot address the loader computes"
              % (7 * len(A) - bad, 7 * len(A)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
