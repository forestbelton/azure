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

TWO SOURCES, because a fully landed block has no nonmatchings left. `--source
nonmatchings` reads `asm/nonmatchings/*/*.s`; `--source objects` reads the
relocations out of `build/csrc/*.o`, which is sound only because the container
builds BYTE-IDENTICAL -- every GPREL16 there is a `$gp`-relative instruction in
the reference. `auto` prefers the listings and falls back to the objects.

AND TWO REPORTS, because the default one overstates what is known.
`--cores` uses POSITIVE evidence only -- the interval spanned by the functions
that reach one symbol `$gp`-relatively is provably one unit -- and reports the
gaps as gaps. On slus_006.14, measured 2026-11-14:

    67 of 1053 functions make a $gp-relative access at all
    29 proven cores covering 89 functions, 8.5%
    the widest gap between two cores is 170 functions

So this signal does NOT determine a carve of that executable; it pins 29
cores and leaves 964 functions in windows a boundary could be anywhere in.
Read `--cores` before believing the default report's run list.
"""

import argparse
import collections
import pathlib
import re
import subprocess
from typing import TypedDict

ROOT = pathlib.Path(__file__).resolve().parent.parent
INSN = re.compile(r"^\s*/\* \w+ \w+ \w+ \*/\s+(\S+)\s*(.*?)\s*$")
GPREL = re.compile(r"%gp_rel\(([\w.]+)\)")
ABS = re.compile(r"%(?:hi|lo)\(([\w.]+)\)")

ScanTuple = tuple[int, str, str, set[str], set[str]]


def scan_nonmatchings() -> list[ScanTuple]:
    """[(addr, name, unit, gp_syms, abs_syms)] from asm/nonmatchings/.

    The original source, and the only one for a block that still has asm
    stubs. It reads nothing for a block that is fully landed, which is what
    `scan_objects` is for.
    """
    out: list[ScanTuple] = []
    for f in sorted((ROOT / "asm/nonmatchings").glob("*/*.s")):
        m = re.fullmatch(r"func_([0-9A-Fa-f]{8})", f.stem)
        if not m:
            continue
        gp, ab = set[str](), set[str]()
        for line in f.read_text().splitlines():
            mi = INSN.match(line)
            if not mi:
                continue
            gp.update(GPREL.findall(line))
            ab.update(ABS.findall(line))
        out.append((int(m.group(1), 16), f.stem, f.parent.name, gp, ab - gp))
    out.sort()
    return out


def units_of(container: str) -> list[str]:
    """The container's `c` subsegment names, in link order."""
    y = (ROOT / "splat" / (container + ".yaml")).read_text()
    return [m.group(1)
            for m in re.finditer(r"^\s*- \[0x[0-9a-f]+, c, (\S+)\]", y, re.M)]


def scan_objects(container: str) -> list[ScanTuple]:
    """The same tuples, read out of the COMPILED OBJECTS instead.

    Once a block is fully landed there are no nonmatchings left, and the
    addressing mode has to come from somewhere else. It comes from our own
    objects, and that is sound for exactly one reason: the container builds
    BYTE-IDENTICAL, so every GPREL16 relocation in `build/csrc/*.o` is a
    `$gp`-relative instruction in the reference and every HI16/LO16 is an
    absolute one. Check `make check` before trusting a run of this.

    Relocations are attributed to the enclosing function by offset, which
    needs the object's own `.text` symbol table rather than the listing.
    """
    out: list[ScanTuple] = []
    for unit in units_of(container):
        o = ROOT / "build/csrc" / (unit + ".o")
        if not o.exists():
            continue
        funcs = []
        for ln in subprocess.run(
                ["mips64-linux-gnu-nm", "--defined-only", str(o)],
                capture_output=True, text=True).stdout.splitlines():
            p = ln.split()
            if len(p) == 3 and p[1] in "tT" and re.fullmatch(
                    r"func_[0-9A-Fa-f]{8}", p[2]):
                funcs.append((int(p[0], 16), p[2]))
        funcs.sort()
        if not funcs:
            continue
        per: dict[str, tuple[set[str], set[str]]] = {
            n: (set(), set()) for _, n in funcs}
        for ln in subprocess.run(
                ["mips64-linux-gnu-objdump", "-r", "-j", ".text", str(o)],
                capture_output=True, text=True).stdout.splitlines():
            m = re.match(r"([0-9a-f]+)\s+R_MIPS_(\w+)\s+(\S+)", ln)
            if not m:
                continue
            off, kind, sym = int(m.group(1), 16), m.group(2), m.group(3)
            if kind not in ("GPREL16", "HI16", "LO16"):
                continue
            owner = None
            for a, n in funcs:
                if a > off:
                    break
                owner = n
            if owner is None:
                continue
            per[owner][0 if kind == "GPREL16" else 1].add(sym)
        for _, n in funcs:
            gp, ab = per[n]
            out.append((int(n[5:], 16), n, unit, gp, ab - gp))
    out.sort()
    return out


def scan(source: str = "auto", container: str = "slus_006.14") -> list[ScanTuple]:
    if source == "nonmatchings":
        return scan_nonmatchings()
    if source == "objects":
        return scan_objects(container)
    out = scan_nonmatchings()
    return out if out else scan_objects(container)


Run = TypedDict(
    "Run",
    {
        "funcs": list[tuple[int, str]],
        "gp": set[str],
        "abs": set[str],
        "blind": int,
        "vetoes": int,
    },
)


def group(funcs: list[ScanTuple]) -> list[Run]:
    """Merge consecutive functions into runs, using both kinds of evidence.

    Positive: sharing a $gp-relative symbol puts two functions in the same
    unit. Negative, and just as useful: reaching a symbol *absolutely* proves
    a function is NOT in the unit that defines it, so a function carrying no
    $gp-relative evidence of its own still cannot be attached to a run that
    owns a symbol it addresses absolutely.
    """
    runs: list[Run] = []
    for addr, name, _unit, gp, ab in funcs:
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
            runs.append(
                {
                    "funcs": [(addr, name)],
                    "gp": set(gp),
                    "abs": set(ab),
                    "blind": 0 if gp else 1,
                    "vetoes": 1 if vetoed else 0,
                }
            )
    return runs


def cores(funcs: list[ScanTuple]) -> list[tuple[int, int, set[str]]]:
    """The runs POSITIVE evidence alone proves, as (lo, hi, symbols).

    `group()` uses negative evidence too -- a function addressing a symbol
    absolutely is not in the unit that defines it -- and that is where it can
    be wrong, because the original source could declare a symbol `extern`
    ABOVE its own definition and get the absolute form inside the defining
    unit. docs/addressing-and-translation-units.md, "Proved: one unit CAN mix
    both modes". Run `group()` and this against each other: where they
    disagree, this one is the floor.

    Positive evidence alone says: every function that reaches symbol `s`
    `$gp`-relatively is in the unit that defines `s`, and the linker lays a
    unit out contiguously, so the whole ADDRESS INTERVAL spanned by those
    functions is one unit. Merging the overlapping intervals gives runs that
    are proven, and gaps that are not evidence of anything.
    """
    span: dict[str, list[int]] = collections.defaultdict(list)
    for addr, _name, _unit, gp, _ab in funcs:
        for s in gp:
            span[s].append(addr)
    out: list[tuple[int, int, set[str]]] = []
    for lo, hi, s in sorted((min(v), max(v), s) for s, v in span.items()):
        if out and lo <= out[-1][1]:
            out[-1] = (out[-1][0], max(out[-1][1], hi), out[-1][2] | {s})
        else:
            out.append((lo, hi, {s}))
    return out


def report_cores(funcs: list[ScanTuple]) -> None:
    import bisect

    addrs = [f[0] for f in funcs]
    unit = {f[0]: f[2] for f in funcs}
    cs = cores(funcs)
    covered = 0
    nsyms = len({s for f in funcs for s in f[3]})
    nwit = sum(1 for f in funcs if f[3])
    print("%d of %d function(s) make a $gp-relative access at all, naming %d"
          " symbol(s) -> %d PROVEN core(s)\n" % (nwit, len(funcs), nsyms, len(cs)))
    print("%-24s %-5s %-20s %s" % ("core", "fns", "current unit(s)", "unproven gap after"))
    for i, (lo, hi, _ss) in enumerate(cs):
        j0, j1 = bisect.bisect_left(addrs, lo), bisect.bisect_right(addrs, hi)
        covered += j1 - j0
        us = sorted({unit[a] for a in addrs[j0:j1]})
        nxt = cs[i + 1][0] if i + 1 < len(cs) else None
        gap = (bisect.bisect_left(addrs, nxt) if nxt else len(addrs)) - j1
        print("%#010x..%#010x %-5d %-20s %d function(s)" % (
            lo, hi, j1 - j0, ",".join(us)[:20], gap))
    print("\n%d of %d function(s) are inside a proven core (%.1f%%); the other"
          " %d are in gaps, and a unit boundary can be anywhere in one."
          % (covered, len(funcs), 100.0 * covered / len(funcs), len(funcs) - covered))
    gaps = [bisect.bisect_left(addrs, cs[i + 1][0])
            - bisect.bisect_right(addrs, cs[i][1]) for i in range(len(cs) - 1)]
    if gaps:
        print("widest gap BETWEEN two cores: %d function(s); after the last"
              " core: %d" % (max(gaps),
                             len(addrs) - bisect.bisect_right(addrs, cs[-1][1])))


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("-n", "--limit", type=int, default=30)
    ap.add_argument(
        "--min-size",
        type=int,
        default=1,
        help="only show runs with at least this many functions",
    )
    ap.add_argument(
        "--source",
        choices=("auto", "nonmatchings", "objects"),
        default="auto",
        help="where the addressing modes come from; `auto` prefers"
             " asm/nonmatchings and falls back to the compiled objects,"
             " which is what a fully landed block needs",
    )
    ap.add_argument(
        "--container",
        default="slus_006.14",
        help="which splat yaml names the `c` units to read, for --source objects",
    )
    ap.add_argument(
        "--cores",
        action="store_true",
        help="report only what POSITIVE evidence proves, and how much of the"
             " block it leaves unproven",
    )
    args = ap.parse_args()

    funcs = scan(args.source, args.container)
    if not funcs:
        raise SystemExit(
            "no per-function evidence: asm/nonmatchings is empty (run"
            " `make splat`) and build/csrc has no objects (run `make`)")
    if args.cores:
        report_cores(funcs)
        return 0
    runs = group(funcs)

    print(
        "%d functions -> %d candidate translation unit(s)\n" % (len(funcs), len(runs))
    )
    print(
        "%-24s %-5s %-7s %s" % ("address range", "fns", "blind", "$gp-relative symbols")
    )
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
        print(
            "%#010x..%#010x %-5d %-7d %s"
            % (
                lo,
                hi,
                len(r["funcs"]),
                r["blind"],
                ", ".join(syms[:4])
                + (" +%d" % (len(syms) - 4) if len(syms) > 4 else "")
                or "-",
            )
        )

    # A symbol reached $gp-relatively from two different runs contradicts the
    # model: it can only be defined in one unit. Report it rather than hide it.
    owner: dict[str, int] = {}
    clashes: dict[str, list[int]] = collections.defaultdict(list)
    for i, r in enumerate(runs):
        for s in r["gp"]:
            if s in owner and owner[s] != i:
                clashes[s].extend([owner[s], i])
            owner.setdefault(s, i)
    if clashes:
        print(
            "\n%d symbol(s) reached $gp-relatively from more than one run --"
            " those runs should merge, or the grouping is wrong:" % len(clashes)
        )
        for s, idxs in sorted(clashes.items())[:10]:
            spans = [
                "%#x..%#x" % (runs[i]["funcs"][0][0], runs[i]["funcs"][-1][0])
                for i in sorted(set(idxs))
            ]
            print("  %-18s %s" % (s, ", ".join(spans)))
    else:
        print(
            "\nNo symbol is reached $gp-relatively from two runs, so the"
            " grouping is self-consistent."
        )

    # Cross-check: a symbol a run reaches absolutely must NOT be defined in it.
    bad: list[tuple[int, str]] = []
    for i, r in enumerate(runs):
        for s in r["abs"] & r["gp"]:
            bad.append((i, s))
    if bad:
        print(
            "\n%d run(s) reach a symbol both ways, which should not happen:" % len(bad)
        )
        for i, s in bad[:8]:
            print("  run %#x: %s" % (runs[i]["funcs"][0][0], s))

    # Independent corroboration. Each object's .sdata/.sbss is laid out in the
    # same order as its .text, so if these runs really are translation units
    # their $gp-relative symbol addresses should climb with their code
    # addresses. That check uses nothing the grouping was built from.
    for family, lo, hi in (
        ("sdata", 0x80080994, 0x80081437),
        ("sbss", 0x80081438, 0x800815BF),
    ):
        seq: list[tuple[int, int]] = []
        for r in runs:
            addrs = [
                int(m.group(1), 16)
                for m in (re.fullmatch(r"D_([0-9A-Fa-f]{8})", s_) for s_ in r["gp"])
                if m and lo <= int(m.group(1), 16) <= hi
            ]
            if addrs:
                seq.append((r["funcs"][0][0], min(addrs)))
        if len(seq) < 2:
            continue
        inversions = sum(1 for (_, a), (_, b) in zip(seq, seq[1:]) if b < a)
        print(
            "\n%s: %d run(s) carry symbols; %d inversion(s) against code order"
            % (family, len(seq), inversions)
        )
        if not inversions:
            print(
                "  symbol order matches code order exactly, which is what a"
                " per-object layout predicts"
            )

    sized = [r for r in runs if len(r["funcs"]) >= args.min_size]
    vetoed = sum(r.get("vetoes", 0) for r in runs)
    print(
        "\n%d run(s) with >=%d function(s); %d function(s) carry no"
        " $gp-relative evidence of their own"
        % (len(sized), args.min_size, sum(r["blind"] for r in runs))
    )
    print(
        "%d boundary/ies forced by negative evidence -- a function"
        " addressing absolutely a symbol the previous run owns" % vetoed
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
