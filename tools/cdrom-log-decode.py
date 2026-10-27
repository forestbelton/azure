#!/usr/bin/env python3
"""Turn PCSX-Redux's CD-ROM command log into disc positions we can act on.

PCSX-Redux logs every seek as

    CD-Rom: 800xxxxx.yyyyyyyy] Command: CdlSetloc 02:46:10

with the minute/second/frame in BCD, which is what the hardware takes. This
converts each one to an LBA and names the container it lands in, so a play
session becomes a list of (container, offset) pairs -- the input for adding
splat segments.

Capture the log with (enable "Log CD-ROM commands" in the Log window's
Special menu, and the CDROM class in its Enabled menu):

    PCSX-Redux.AppImage -stdout -iso az.bin -run 2>&1 | tee cdrom.log

then:

    python3 tools/cdrom-log-decode.py cdrom.log

This reads the CD-ROM layer, so it works regardless of the CPU core and needs
no breakpoints.
"""
import argparse
import collections
import pathlib
import re
import sys

import yaml

SETLOC = re.compile(r"CdlSetloc\s+([0-9a-fA-F]{2}):([0-9a-fA-F]{2}):([0-9a-fA-F]{2})")

# A CD's first sector is MSF 00:02:00, so LBA = frames - 150.
PREGAP = 150


def bcd(b: str) -> int:
    """BCD byte as printed by %02x -- '43' means decimal 43, not 0x43."""
    v = int(b, 16)
    hi, lo = v >> 4, v & 0xF
    if hi > 9 or lo > 9:
        raise ValueError(f"not BCD: {b}")
    return hi * 10 + lo


def msf_to_lba(m: str, s: str, f: str) -> int:
    return (bcd(m) * 60 + bcd(s)) * 75 + bcd(f) - PREGAP


def containers(spec_path: pathlib.Path):
    spec = yaml.safe_load(spec_path.read_text())
    out = []
    for e in spec["files"]:
        n = (e["size"] + 2047) // 2048
        out.append((e["sector"], e["sector"] + n - 1, e["name"]))
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("log", type=pathlib.Path)
    ap.add_argument("--spec", type=pathlib.Path, default=pathlib.Path("spec.yaml"))
    ap.add_argument("--all", action="store_true",
                    help="list every seek, not just the distinct ones")
    args = ap.parse_args()

    ranges = containers(args.spec)

    def who(lba):
        for a, b, name in ranges:
            if a <= lba <= b:
                return name, (lba - a) * 2048
        return None, None

    seeks = []
    for line in args.log.read_text(errors="replace").splitlines():
        m = SETLOC.search(line)
        if not m:
            continue
        try:
            seeks.append(msf_to_lba(*m.groups()))
        except ValueError as e:
            print(f"skipping malformed MSF: {e}", file=sys.stderr)

    if not seeks:
        print("No CdlSetloc lines found. Was 'Log CD-ROM commands' enabled in "
              "the Log window's Special menu, and the CDROM class in Enabled?")
        return 1

    print(f"{len(seeks)} seeks, {len(set(seeks))} distinct\n")
    counts = collections.Counter(seeks)
    order = seeks if args.all else sorted(set(seeks))
    seen = set()
    by_file = collections.Counter()
    print(f"  {'LBA':>7}  {'hits':>5}  source")
    for lba in order:
        if not args.all:
            if lba in seen:
                continue
            seen.add(lba)
        name, off = who(lba)
        by_file[name or "(outside any file)"] += 1
        where = f"{name}+{off:#x}" if name else "outside any file"
        print(f"  {lba:>7}  {counts[lba]:>5}  {where}")

    print("\ndistinct seeks per container:")
    for name, n in by_file.most_common():
        print(f"  {name:<22} {n}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
