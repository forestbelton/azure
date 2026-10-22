#!/usr/bin/env python3
"""Rank the not-yet-decompiled functions by how hard they look.

Reads every per-function file under asm/nonmatchings/ and scores it, so
picking what to do next is mechanical rather than a hunt. A function is
counted as done when its C file no longer has an INCLUDE_ASM for it.

Difficulty is a rough proxy, in rising order of pain: instruction count,
then calls (each needs a known signature), then branches (control flow is
where matching gets fiddly), then whether it touches $gp-relative data or
contains inline .word data.
"""

import argparse
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
INSN = re.compile(r"^\s*/\* \w+ \w+ \w+ \*/\s+(\S+)")


def scan(path: pathlib.Path) -> dict:
    insns = calls = branches = words = gprel = forma = 0
    for line in path.read_text().splitlines():
        if ".word" in line:
            words += 1
            continue
        m = INSN.match(line)
        if not m:
            continue
        insns += 1
        mn = m.group(1)
        if mn in ("jal", "jalr"):
            calls += 1
        elif mn[0] in "bj" and mn != "jr":
            branches += 1
        if "%gp_rel" in line:
            gprel += 1
        # Form A: the address of a global taken into a register, then reached
        # with displacements. gcc will not emit this for direct global access,
        # so these functions are currently unmatchable -- see fieldscan notes.
        if mn in ("addiu", "addu", "addi") and "%lo(" in line:
            forma += 1
    return {
        "insns": insns, "calls": calls, "branches": branches,
        "words": words, "gprel": gprel, "forma": forma,
    }


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("-n", "--limit", type=int, default=25)
    ap.add_argument("--leaves", action="store_true",
                    help="only functions with no calls")
    ap.add_argument("--matchable", action="store_true",
                    help="exclude functions using the register-held base "
                         "idiom, which does not reproduce from C yet")
    ap.add_argument("--done", action="store_true",
                    help="list already-decompiled functions instead")
    args = ap.parse_args()

    rows = []
    for d in sorted((ROOT / "asm/nonmatchings").glob("*")):
        if not d.is_dir():
            continue
        cfile = ROOT / "csrc" / f"{d.name}.c"
        src = cfile.read_text() if cfile.exists() else ""
        for f in sorted(d.glob("*.s")):
            name = f.stem
            stubbed = re.search(rf"INCLUDE_ASM\([^)]*,\s*{re.escape(name)}\)", src)
            info = scan(f)
            info.update(name=name, unit=d.name, done=not stubbed)
            rows.append(info)

    total = len(rows)
    done = sum(1 for r in rows if r["done"])
    print(f"{done}/{total} functions decompiled "
          f"({100 * done / total if total else 0:.1f}%)\n")

    rows = [r for r in rows if r["done"] == bool(args.done)]
    if args.leaves:
        rows = [r for r in rows if r["calls"] == 0]
    if args.matchable:
        rows = [r for r in rows if r["forma"] == 0]
    rows.sort(key=lambda r: (r["words"] > 0, r["forma"] > 0, r["calls"],
                             r["branches"], r["gprel"], r["insns"]))

    print(f"{'function':<22}{'unit':<12}{'insns':>6}{'call':>5}{'br':>4}"
          f"{'gp':>4}{'fA':>4}{'data':>5}")
    for r in rows[:args.limit]:
        print(f"  {r['name']:<20}{r['unit']:<12}{r['insns']:>6}{r['calls']:>5}"
              f"{r['branches']:>4}{r['gprel']:>4}{r['forma']:>4}{r['words']:>5}")
    if len(rows) > args.limit:
        print(f"  ... and {len(rows) - args.limit} more")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
