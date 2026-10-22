#!/usr/bin/env python3
"""Propose original translation-unit boundaries from $gp-relative accesses.

ASPSX could only use $gp for a symbol whose size it knew, which means only for
a symbol defined in the translation unit being assembled. So a function that
reaches a global $gp-relatively was compiled in the unit that *defined* that
global, and one that reaches the same global absolutely was not. See
docs/addressing-and-translation-units.md.

That makes $gp-relative accesses a membership signal. Two independent facts
constrain the grouping:

  * functions sharing a $gp-relative symbol belong to the same unit, and
  * the linker lays each object's text out contiguously, so a unit must be a
    contiguous run of functions in address order.

The second is what makes this worth doing rather than guessing: it is a strong
check the first has to satisfy, and where the two disagree the output says so
instead of smoothing it over.
"""

import argparse
import collections
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parent.parent
INSN = re.compile(r"^\s*/\* \w+ \w+ \w+ \*/\s+(\S+)\s*(.*?)\s*$")
GPREL = re.compile(r"%gp_rel\(([\w.]+)\)")
ABS = re.compile(r"%(?:hi|lo)\(([\w.]+)\)")


def scan():
    """[(addr, name, unit, gp_syms, abs_syms)] in address order."""
    out = []
    for f in sorted((ROOT / "asm/nonmatchings").glob("*/*.s")):
        m = re.fullmatch(r"func_([0-9A-Fa-f]{8})", f.stem)
        if not m:
            continue
        gp, ab = set(), set()
        for line in f.read_text().splitlines():
            mi = INSN.match(line)
            if not mi:
                continue
            gp.update(GPREL.findall(line))
            ab.update(ABS.findall(line))
        out.append((int(m.group(1), 16), f.stem, f.parent.name, gp, ab - gp))
    out.sort()
    return out


def group(funcs):
    """Merge consecutive functions into runs, using both kinds of evidence.

    Positive: sharing a $gp-relative symbol puts two functions in the same
    unit. Negative, and just as useful: reaching a symbol *absolutely* proves
    a function is NOT in the unit that defines it, so a function carrying no
    $gp-relative evidence of its own still cannot be attached to a run that
    owns a symbol it addresses absolutely.
    """
    runs = []
    for addr, name, unit, gp, ab in funcs:
        prev = runs[-1] if runs else None
        # Negative evidence vetoes attachment even when there is nothing
        # positive to go on.
        vetoed = bool(prev and (ab & prev["gp"]))
        if prev and not vetoed and (gp & prev["gp"] or not gp):
            # Shares a symbol with the run, or carries no evidence at all and
            # so is left attached to its neighbour rather than split off.
            r = runs[-1]
            r["funcs"].append((addr, name))
            r["gp"] |= gp
            r["abs"] |= ab
            r["blind"] += 0 if gp else 1
            r["vetoes"] = r.get("vetoes", 0)
        else:
            runs.append({"funcs": [(addr, name)], "gp": set(gp),
                         "abs": set(ab), "blind": 0 if gp else 1,
                         "vetoes": 1 if vetoed else 0})
    return runs


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("-n", "--limit", type=int, default=30)
    ap.add_argument("--min-size", type=int, default=1,
                    help="only show runs with at least this many functions")
    args = ap.parse_args()

    funcs = scan()
    if not funcs:
        raise SystemExit("no per-function asm; run `make splat`")
    runs = group(funcs)

    print("%d functions -> %d candidate translation unit(s)\n"
          % (len(funcs), len(runs)))
    print("%-24s %-5s %-7s %s" % ("address range", "fns", "blind", "$gp-relative symbols"))
    shown = 0
    for r in runs:
        if len(r["funcs"]) < args.min_size:
            continue
        shown += 1
        if shown > args.limit:
            break
        lo = r["funcs"][0][0]
        hi = r["funcs"][-1][0]
        syms = sorted(r["gp"])
        print("%#010x..%#010x %-5d %-7d %s"
              % (lo, hi, len(r["funcs"]), r["blind"],
                 ", ".join(syms[:4]) + (" +%d" % (len(syms) - 4) if len(syms) > 4 else "")
                 or "-"))

    # A symbol reached $gp-relatively from two different runs contradicts the
    # model: it can only be defined in one unit. Report it rather than hide it.
    owner = {}
    clashes = collections.defaultdict(list)
    for i, r in enumerate(runs):
        for s in r["gp"]:
            if s in owner and owner[s] != i:
                clashes[s].extend([owner[s], i])
            owner.setdefault(s, i)
    if clashes:
        print("\n%d symbol(s) reached $gp-relatively from more than one run --"
              " those runs should merge, or the grouping is wrong:" % len(clashes))
        for s, idxs in sorted(clashes.items())[:10]:
            spans = ["%#x..%#x" % (runs[i]["funcs"][0][0], runs[i]["funcs"][-1][0])
                     for i in sorted(set(idxs))]
            print("  %-18s %s" % (s, ", ".join(spans)))
    else:
        print("\nNo symbol is reached $gp-relatively from two runs, so the"
              " grouping is self-consistent.")

    # Cross-check: a symbol a run reaches absolutely must NOT be defined in it.
    bad = []
    for i, r in enumerate(runs):
        for s in r["abs"] & r["gp"]:
            bad.append((i, s))
    if bad:
        print("\n%d run(s) reach a symbol both ways, which should not happen:"
              % len(bad))
        for i, s in bad[:8]:
            print("  run %#x: %s" % (runs[i]["funcs"][0][0], s))

    # Independent corroboration. Each object's .sdata/.sbss is laid out in the
    # same order as its .text, so if these runs really are translation units
    # their $gp-relative symbol addresses should climb with their code
    # addresses. That check uses nothing the grouping was built from.
    for family, lo, hi in (("sdata", 0x80080994, 0x80081437),
                           ("sbss", 0x80081438, 0x800815BF)):
        seq = []
        for r in runs:
            addrs = [int(m.group(1), 16)
                     for m in (re.fullmatch(r"D_([0-9A-Fa-f]{8})", s_) for s_ in r["gp"])
                     if m and lo <= int(m.group(1), 16) <= hi]
            if addrs:
                seq.append((r["funcs"][0][0], min(addrs)))
        if len(seq) < 2:
            continue
        inversions = sum(1 for (_, a), (_, b) in zip(seq, seq[1:]) if b < a)
        print("\n%s: %d run(s) carry symbols; %d inversion(s) against code order"
              % (family, len(seq), inversions))
        if not inversions:
            print("  symbol order matches code order exactly, which is what a"
                  " per-object layout predicts")

    sized = [r for r in runs if len(r["funcs"]) >= args.min_size]
    vetoed = sum(r.get("vetoes", 0) for r in runs)
    print("\n%d run(s) with >=%d function(s); %d function(s) carry no"
          " $gp-relative evidence of their own"
          % (len(sized), args.min_size, sum(r["blind"] for r in runs)))
    print("%d boundary/ies forced by negative evidence -- a function"
          " addressing absolutely a symbol the previous run owns" % vetoed)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
