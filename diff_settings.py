"""Configuration for asm-differ (https://github.com/simonlindholm/asm-differ).

Run its diff.py from this directory, e.g.

    vendor//asm-differ/diff.py -mow func_8003E12C

-m rebuilds via make first, -o diffs against the original object, -w watches
for changes. The byte-identical asm-only build is the baseline, so any
divergence shown is caused by whatever is currently being decompiled.
"""

from typing import Any


def apply(config: Any, args: Any) -> None:
    # mipsel: little-endian, -m mips:3000 -- the R3000A the PSX actually has.
    # Plain "mips" is big-endian and disassembles the image into nonsense.
    config["arch"] = "mipsel"
    config["baseimg"] = "data/slus_006.14"
    config["myimg"] = "build/slus_006.14"
    config["mapfile"] = "build/slus_006.14.map"
    config["map_format"] = "gnu"
    config["build_dir"] = "build"
    config["makeflags"] = ["build/slus_006.14"]
    config["source_directories"] = ["csrc", "include", "asm"]
    # Fedora has no mipsel-linux-gnu binutils; mips64 reads elf32ltsmip fine.
    config["objdump_executable"] = "mips64-linux-gnu-objdump"
