#!/usr/bin/env python3
"""Redistribute decompiled C across the current translation-unit layout.

Carving a new `c` subsegment moves functions between units, but splat only
creates C files that do not exist yet -- it will not move a function body that
has already been decompiled, and it leaves behind INCLUDE_ASM lines for
functions the unit no longer owns. Both produce duplicate or dangling symbols.

This rewrites every csrc/<unit>.c so it contains exactly the functions that
asm/nonmatchings/<unit>/ says it owns: the decompiled body where one exists
anywhere in the tree, an INCLUDE_ASM stub otherwise. Hand-written code is
preserved by being moved, never regenerated.
"""

import argparse
import pathlib
import re
import yaml

ROOT = pathlib.Path(__file__).resolve().parent.parent
CSRC = ROOT / "csrc"
NONMATCH = ROOT / "asm/nonmatchings"

Range = tuple[int, int | None, str]


def unit_ranges() -> list[Range]:
    """[(lo, hi, name)] for every `c` subsegment, from the splat config.

    asm/nonmatchings cannot be used for this: splat only emits per-function
    assembly for functions that are still stubbed, so a unit whose functions
    are all decompiled has no directory at all.
    """
    doc = yaml.safe_load((ROOT / "splat/slus_006.14.yaml").read_text())
    segs = doc["segments"]
    out: list[Range] = []
    for i, seg in enumerate(segs):
        if not isinstance(seg, dict) or not seg.get("subsegments"):
            continue
        tail = next((x[0] for x in segs[i + 1 :] if isinstance(x, list) and x), None)
        ents = [e for e in seg["subsegments"] if isinstance(e, list) and e]
        for j, e in enumerate(ents):
            if len(e) > 2 and e[1] == "c":
                hi = ents[j + 1][0] if j + 1 < len(ents) else tail
                out.append((e[0], hi, e[2]))
    return out


VRAM, HDR = 0x8002D000, 0x800


def file_offset(name: str) -> int:
    return int(name[5:], 16) - VRAM + HDR


DEF = re.compile(
    r"^(?:[A-Za-z_][\w \t*]*?)\b(func_[0-9A-Fa-f]{8})\s*\([^;]*?\)\s*\{", re.M
)
STUB = re.compile(r"^INCLUDE_ASM\([^)]*,\s*(func_[0-9A-Fa-f]{8})\);\s*$", re.M)


def extract_bodies(text: str) -> dict[str, str]:
    """{name: source} for every function defined (not stubbed) in this file."""
    out: dict[str, str] = {}
    for m in DEF.finditer(text):
        name = m.group(1)
        i = text.index("{", m.start())
        depth = 0
        for j in range(i, len(text)):
            if text[j] == "{":
                depth += 1
            elif text[j] == "}":
                depth -= 1
                if depth == 0:
                    out[name] = text[m.start() : j + 1]
                    break
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()

    # Gather every decompiled body, wherever it currently lives.
    bodies: dict[str, str] = {}
    for f in sorted(CSRC.glob("*.c")):
        for name, src in extract_bodies(f.read_text()).items():
            bodies.setdefault(name, src)
    print("found %d decompiled function(s)" % len(bodies))

    # Every function we know about: decompiled bodies plus remaining stubs.
    known = set(bodies)
    for f in sorted(NONMATCH.glob("*/*.s")):
        if re.fullmatch(r"func_[0-9A-Fa-f]{8}", f.stem):
            known.add(f.stem)

    placed = stubbed = 0
    for lo, hi, unit in unit_ranges():
        if hi is None:
            continue
        owned = sorted((n for n in known if lo <= file_offset(n) < hi), key=file_offset)
        if not owned:
            print("  %s owns no functions" % unit)
            continue
        parts = ['#include "common.h"', ""]
        for name in owned:
            if name in bodies:
                parts.append(bodies[name])
                placed += 1
            else:
                parts.append('INCLUDE_ASM("asm/nonmatchings/%s", %s);' % (unit, name))
                stubbed += 1
            parts.append("")
        text = "\n".join(parts)
        target = CSRC / (unit + ".c")
        if args.dry_run:
            old = target.read_text() if target.exists() else ""
            if old != text:
                print("  would rewrite %s (%d owned)" % (target.name, len(owned)))
        else:
            target.write_text(text)

    units = {u for _, _, u in unit_ranges()}
    orphans = [f.name for f in sorted(CSRC.glob("*.c")) if f.stem not in units]
    if orphans:
        print("note: csrc files with no `c` subsegment: %s" % ", ".join(orphans))
    print("placed %d decompiled, stubbed %d" % (placed, stubbed))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
