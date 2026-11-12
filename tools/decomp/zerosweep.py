#!/usr/bin/env python3
"""Find declared ranges that are ENTIRELY zero in the reference container.

A run of stored zeros does not need an asset, a listing or an object. splat's
`pad` type declares it in one line -- no name, no files -- emitting only a
`. += 0x<size>;` into the linker script, which advances the running `__romPos`
so every later segment keeps its LMA. See splat/main.bin.yaml at 0x24420 for
the two forms it takes and the one hazard (a standalone `pad` must never be a
container's last segment).

THIS EXISTS BECAUSE THE OBVIOUS SWEEP IS WRONG. Scanning assets/*.bin for
all-zero files only ever finds `bin` subsegments -- it cannot see a zero run
declared as `data`, which is how 0x268b6c hid. The ranges come from the yaml
and the bytes from data/, so every declared type is covered equally.

Reports a range only if it is all zero AND not already a `pad`, so a clean run
means there is nothing left to convert. Exits 1 when it finds any, to be
usable as a check.
"""

import io
import os
import sys

import yaml

PROJ = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

# yaml config -> the reference container it describes
CONTAINERS = {
    "splat/slus_006.14.yaml": "data/slus_006.14",
    "splat/town.bin.yaml": "data/town/town.bin",
    "splat/main.bin.yaml": "data/main/main.bin",
    "splat/dungeon.bin.yaml": "data/dungeon/dungeon.bin",
}

# Types that already declare a zero run, or that cannot hold stored bytes at
# all, so finding them "all zero" is not actionable.
SKIP_TYPES = {"pad", "bss", "sbss", "linker_offset"}


def flatten(segments, out):
    """Collect (start, type, name) for every segment and subsegment."""
    for seg in segments:
        if isinstance(seg, list):
            start = seg[0]
            if not isinstance(start, int):
                continue
            out.append((start, seg[1] if len(seg) > 1 else "(end)",
                        seg[2] if len(seg) > 2 else ""))
        elif isinstance(seg, dict):
            out.append((seg["start"], seg.get("type", "?"), seg.get("name", "")))
            flatten(seg.get("subsegments", []), out)


def sweep(yaml_path, rom_path):
    cfg = yaml.safe_load(io.open(os.path.join(PROJ, yaml_path), encoding="utf-8"))
    ranges = []
    flatten(cfg["segments"], ranges)
    # A subsegment and its parent share a start; dedupe so the parent's own
    # entry does not swallow the first subsegment's range.
    ranges = sorted(set(ranges), key=lambda r: r[0])

    data = io.open(os.path.join(PROJ, rom_path), "rb").read()

    found = []
    for i, (start, typ, name) in enumerate(ranges):
        end = ranges[i + 1][0] if i + 1 < len(ranges) else len(data)
        if end <= start or typ in SKIP_TYPES:
            continue
        if not any(data[start:end]):
            found.append((start, end, typ, name))
    return found, len(data)


def main():
    total = 0
    for yaml_path, rom_path in CONTAINERS.items():
        if not os.path.exists(os.path.join(PROJ, rom_path)):
            print("SKIP %s (no %s)" % (yaml_path, rom_path))
            continue
        found, size = sweep(yaml_path, rom_path)
        print("%-28s %10d bytes  %d all-zero range(s)"
              % (rom_path, size, len(found)))
        for start, end, typ, name in found:
            print("    0x%-9X..0x%-9X %9d bytes  type=%-10s %s"
                  % (start, end, end - start, typ, name))
            print("        -> candidate for `- [0x%x, pad]`" % start)
        total += len(found)

    print()
    if total:
        print("%d range(s) could be declared `pad`." % total)
        return 1
    print("No declared range is entirely zero; nothing to convert.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
