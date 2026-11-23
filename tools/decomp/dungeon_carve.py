#!/usr/bin/env python3
"""Emit splat segments for every code object in DUNGEON.BIN.

`dungeon_objects.py` counts them; this one carves them. It writes the YAML
block that goes between `- [0xf7700, bin]` and the container's end marker in
splat/dungeon.bin.yaml, one `code` segment per object with a `bin` segment
filling every gap.

    python3 tools/decomp/dungeon_carve.py            # the YAML block
    python3 tools/decomp/dungeon_carve.py --check    # the boundary report

Each object is laid out data / text, and the two boundaries are measured:

  HEADER.  Every object in both families begins with a table of pointers into
  its own text. The data subsegment runs to the last such pointer.

  TEXT START.  The word after that last pointer, then advanced past any word
  that does not decode as a MIPS instruction. The naive rule -- "the first
  `addiu $sp, $sp, -N`" -- is WRONG for nine of the 116, because an object can
  open with a LEAF function that has no prologue (s22 at +0x28, b22 at +0x6c,
  b31f at +0x104, and six more), and the naive rule swallows those whole
  functions into the data subsegment where nothing disassembles them.

  TEXT END.  The last `jr $ra` plus its delay slot. Checked: no object's last
  function has an implausible opcode in it, and the long ones are real -- s41
  really does end with a 996-word function, prologue and epilogue intact.

A boundary that is a few words off does NOT break the build: splat emits
`.word` for anything it cannot decode, so the bytes round-trip either way and
tools/check.py still proves the container byte-identical. It costs the first
function its own symbol, which is the same error the dungeon overlay's own
58-word misplacement was, and is corrected the same way -- per object, when
somebody reads it. `--check` prints every object the cross-checks complain
about so the list stays honest.

exclusive_ram_id NAMES THE RAM SPACE, NOT THE OBJECT. splat keys
`overlay_segments[id]` by ROM inside the id, and when an address misses in a
segment's own id it deliberately falls through to every OTHER id (see
util/metadata/segment_manager.py, `_find_symbol_from_overlay_segments`). Giving
each object its own id therefore does the opposite of isolating them: with 116
ids the fallback named a branch label inside s62 `.Llo20_80026BE4`, after the
segment whose window happens to cover the same address, and the link came apart
on hundreds of undefined `.L` symbols. Two ids, one per RAM space, is the
configuration splat is built for -- `dng_slot` for everything in
0x8014C800..0x80176800 and `dng_low` for everything in the low-RAM window,
where lo16, lo20 and the 66 0x80024000 objects all overlap each other.

ONLY ONE COPY OF EACH OBJECT IS CARVED. func_800A0B94's family is stored as
seven relinked copies per object; the other six stay `bin` and relink
verbatim. See docs/dungeon-map.md.
"""
import argparse
import os
import sys

import numpy as np
import rabbitizer

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.environ.get("AZURE_ROOT", os.path.dirname(os.path.dirname(HERE)))
IMG = os.path.join(PROJ, "data/dungeon/dungeon.bin")

BASE_LBA = 12310
JRRA = 0x03E00008
BANK = 0x6000
SLOT0 = 0x80170800
SLOT_LO, SLOT_HI = SLOT0 - 6 * BANK, SLOT0 + BANK
OVL24 = 0x80024000

# MIPS I plus the coprocessor escapes, as a validity sieve. Only used to step
# over data that sits between the pointer header and the first instruction.
# Whether a word can be an instruction. rabbitizer is the decoder spimdisasm
# itself uses, so this agrees with what splat will do with the word -- and it
# rejects things a hand-rolled opcode sieve waves through, `0x01000340` among
# them: funct 0 is `sll`, but `sll` with a non-zero rs is not an encoding, and
# that word is the first of a four-word table that sits between the pointer
# header and the text in b05, b27 and b39.
#
# A valid encoding is not enough on its own. `0x0bb82000` decodes as a perfectly
# good `j`, to 0x8EE08000, which is not RAM -- three words like that are the
# table at the head of s35 and s36, and reading them as the start of the text
# put an undefined `func_8EE08000` in the link. Real code only ever jumps into
# RAM, so the target is checked too.
# rabbitizer is not enough BY ITSELF either: it accepts the MIPS III
# encodings, and `0xfffefffe` is a fine `sd` and `0x0002fffe` a fine
# `dsrl32`. The R3000 has neither, and those two words are a halfword vector
# table in b27. So the word must be an R3000 encoding AND rabbitizer must
# accept the field combination AND, for a jump, the target must be RAM.
RAM = ((0x80010000, 0x80200000), (0x80400000, 0x80800000))
R3000_OPS = (set(range(0x00, 0x14)) | set(range(0x20, 0x27))
             | {0x28, 0x29, 0x2A, 0x2B, 0x2E}
             | set(range(0x30, 0x34)) | set(range(0x38, 0x3C)))
R3000_SPECIAL = ({0x00, 0x02, 0x03, 0x04, 0x06, 0x07, 0x08, 0x09, 0x0C, 0x0D}
                 | {0x10, 0x11, 0x12, 0x13, 0x18, 0x19, 0x1A, 0x1B}
                 | set(range(0x20, 0x28)) | {0x2A, 0x2B})


def plausible(w):
    op = w >> 26
    if op not in R3000_OPS:
        return False
    if op == 0 and (w & 0x3F) not in R3000_SPECIAL:
        return False
    if not rabbitizer.Instruction(w).isValid():
        return False
    if op in (2, 3):
        t = 0x80000000 | ((w & 0x03FFFFFF) << 2)
        return any(lo <= t < hi for lo, hi in RAM)
    return True


def is_pro(w):
    return (w & 0xFFFF0000) == 0x27BD0000 and (w & 0x8000)


def off_of(lba):
    return (lba - BASE_LBA) * 0x800


class Carve:
    def __init__(self, img, name, off, size, vram, lo, hi, ram):
        self.name, self.off, self.size, self.vram = name, off, size, vram
        self.lo, self.hi, self.ram = lo, hi, ram
        self.w = img[off >> 2: (off + size) >> 2]
        jr = [int(i) for i in np.flatnonzero(self.w == JRRA)]
        self.jr = jr
        self.text_end = (jr[-1] + 2) * 4
        # the last pointer into this object's own window below the first `jr $ra`
        # The text starts after BOTH the last pointer into this object's own
        # window and the last word that does not decode -- whichever is later.
        # Neither test alone is enough: a pointer like 0x80173a08 decodes as a
        # perfectly good `lb`, and a halfword vector table decodes as nothing.
        p = -1
        for k in range(jr[0]):
            v = int(self.w[k])
            if lo <= v < hi or not plausible(v):
                p = k
        self.text_start = (p + 1) * 4
        self.seg_end = (self.text_end + 15) & ~15

    def starts(self):
        s = self.text_start >> 2
        return [s] + [k + 2 for k in self.jr if s < k + 2 < self.text_end >> 2]

    def complaints(self):
        out = []
        ts, te = self.text_start >> 2, self.text_end >> 2
        st = set(self.starts())
        if any(self.w[:ts] == JRRA):
            k = int(np.flatnonzero(self.w[:ts] == JRRA)[0])
            out.append("jr $ra at +0x%04x, below text_start" % (k * 4))
        op = self.w[ts:te] >> 26
        miss = set()
        for k in np.flatnonzero(op == 3).tolist():
            t = int(0x80000000 | ((int(self.w[ts + k]) & 0x03FFFFFF) << 2))
            if self.vram <= t < self.vram + self.size and ((t - self.vram) >> 2) not in st:
                miss.add(t - self.vram)
        if miss:
            out.append("%d internal jal targets off a start (%s)"
                       % (len(miss), ", ".join("+0x%x" % m for m in sorted(miss)[:4])))
        seg = self.w[ts:te]
        imp = sum(1 for v in seg.tolist() if not plausible(v))
        if imp > len(seg) * 0.01:
            out.append("%.1f%% of text does not decode" % (100.0 * imp / len(seg)))
        return out

    def yaml(self):
        pre = self.name.split("_")[-1]
        L = ["  - name: ovl_dng_%s" % self.name,
             "    type: code",
             "    start: 0x%x" % self.off,
             "    vram: 0x%08x" % self.vram,
             "    exclusive_ram_id: %s" % self.ram,
             "    symbol_name_format: %s_$VRAM" % pre,
             "    subsegments:"]
        if self.text_start:
            L.append("      - [0x%x, data]" % self.off)
        L.append("      - [0x%x, c, ovl_dng_%s]" % (self.off + self.text_start, self.name))
        if self.seg_end != self.text_end:
            L.append("      - [0x%x, data]" % (self.off + self.text_end))
        return L


def objects(img):
    out = []
    for a in list(range(1, 46)) + [55, 57]:
        out.append(Carve(img, "b%02d" % a, off_of(0x4340 + (a - 1) * 84),
                         BANK, SLOT0, SLOT_LO, SLOT_HI, "dng_slot"))
    out.append(Carve(img, "b31f", off_of(0x569C), 0xC000, SLOT0 - BANK,
                     SLOT_LO, SLOT_HI, "dng_slot"))
    for a in range(1, 110):
        off = off_of(0x607F + (a - 1) * 12)
        if off + BANK > img.nbytes:
            break
        h = int(img[off >> 2])
        if not (OVL24 <= h < OVL24 + BANK):
            continue
        if not is_pro(int(img[(off >> 2) + ((h - OVL24) >> 2)])):
            continue
        out.append(Carve(img, "s%02d" % a, off, BANK, OVL24, OVL24, OVL24 + BANK,
                         "dng_low"))
    # The two singleton overlays. Each begins with its own switch jump tables
    # -- the pointers at lo16+0 land in the MIDDLE of its functions, which is
    # what the yaml's note about "a jump table into ITSELF" is -- and then its
    # text.
    #
    # lo20 STARTS AT 0x1829800, NOT 0x182b000, and so loads at 0x8001E800
    # rather than the 0x80020000 docs/dungeon-map.md section 6 gave it. The
    # delta is the same either way; 0x182b000 was simply where a sweep window
    # happened to begin. Walking back from it, the code runs unbroken through
    # 0x182aff0 to 0x1829814, under a 5-word header at 0x1829800, under 0x600
    # bytes of zero padding to the sector boundary. All five header pointers
    # land inside the object at this base.
    out.append(Carve(img, "lo16", 0x2A3000, 0xD800, 0x80016000, 0x80016000,
                     0x80023800, "dng_low"))
    out.append(Carve(img, "lo20", 0x1829800, 0xB000, 0x8001E800, 0x8001E800,
                     0x80029800, "dng_low"))
    out.sort(key=lambda c: c.off)
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--check", action="store_true")
    a = ap.parse_args()

    img = np.fromfile(IMG, dtype="<u4")
    objs = objects(img)

    if a.check:
        bad = 0
        for c in objs:
            msgs = c.complaints()
            if msgs:
                bad += 1
                print("%-6s 0x%07x text +0x%04x..+0x%04x : %s"
                      % (c.name, c.off, c.text_start, c.text_end, "; ".join(msgs)))
        print("%d objects, %d with a complaint, %d functions, %d words of text"
              % (len(objs), bad, sum(len(c.starts()) for c in objs),
                 sum((c.text_end - c.text_start) // 4 for c in objs)))
        return 0

    prev = 0xF7700
    for c in objs:
        if c.off > prev:
            print("  - [0x%x, bin]" % prev)
        for line in c.yaml():
            print(line)
        prev = c.off + c.seg_end
    print("  - [0x%x, bin]" % prev)
    return 0


if __name__ == "__main__":
    sys.exit(main())
