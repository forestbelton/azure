#!/usr/bin/env python3
"""Recover original translation-unit boundaries for main.bin's card overlay.

`tools/tuscan.py` proposes units from $gp-relative accesses. That signal does
not exist in the overlays: they carry no $gp-relative addressing at all (0 refs
across ~200K instructions), so they were built at -G0 and tuscan has nothing to
read. This is the replacement, and it is stronger than what tuscan could do.

THE TWO BLOCKS. `main.bin+0x25d000` (vram 0x80020000) is the live card overlay,
241 functions, and is what we actually decompile. `main.bin+0x269000` (vram
0x80400000) is a DEBUG BUILD OF THE SAME SOURCE, 263 functions, with asserts
compiled in. It is dead code on retail -- 0x80400000 is 8 MB dev-board RAM --
but it names things the live block does not. See docs/overlays.md.

THREE SIGNALS, in decreasing strength.

1. Assert sites. Each `assert(x)` expands to

       lui   $a0, %hi(ASSERT_FMT)     # "Assertion failed: file \"%s\", line %d\n"
       addiu $a0, $a0, %lo(ASSERT_FMT)
       lui   $a1, %hi(FILENAME)       # "c_server.c", "cardbk.c", ...
       addiu $a1, $a1, %lo(FILENAME)
       jal   printf
        addiu $a2, $zero, LINE        # __LINE__
       jal   exit
        addiu $a0, $zero, 1

   so a site names its translation unit and its source line outright. 37 sites
   across 9 units.

2. Own-rodata references. Each unit's string literals sit in its own object's
   .rodata, and the linker emitted the units' rodata in the same order as their
   text. A function referencing an address inside unit X's rodata run was
   compiled in X. This is what tuscan's $gp signal was a proxy for, available
   directly here. 33 functions, and in this block ZERO of them reference more
   than one unit's rodata.

3. Own-data references, as corroboration only. .bss here holds shared globals,
   not just per-unit statics, so the ranges overlap and it cannot set a
   boundary. Used to argue the head of the block belongs to c_server.c.

And one structural fact constrains all three, as in tuscan.py: the linker lays
each object's text out contiguously, so a unit must be a contiguous run of
functions in address order. Where a signal disagrees with that, this tool says
so rather than smoothing it over.

PROPAGATION TO THE LIVE BLOCK. The two builds are the same source in the same
link order, so their function sequences align. This tool runs an
order-preserving global alignment over masked instruction words (jal targets
and i-type immediates masked out, since those differ between the links) and
carries each unit label across. 171 of the live block's 241 functions pair
directly. The live block has only one assert of its own (`c_server.c`), so
without this it would have no unit map at all.

TWO CAUTIONS about the twin, both found the hard way:

  * Its external `jal` targets are NOT retail engine addresses. It calls printf
    at 0x8007C040 and exit at 0x8007BEF0, which in the shipped executable land
    inside libgte's `rcossin_tbl`. It was linked against a different engine
    build. Use its filenames, line numbers and internal structure; never its
    external addresses.
  * Its last ~23 functions are PSY-Q library stubs, not game code -- 16-byte
    bodies at 16-byte spacing, below `memory.c`. They are not one of the nine
    units.

Usage:
    python3 tools/assertmap.py                  # the twin's unit map
    python3 tools/assertmap.py --sites          # every assert site
    python3 tools/assertmap.py --live           # propagated map for 0x80020000
    python3 tools/assertmap.py --live --json out.json
"""

import argparse
import array
import bisect
import collections
import difflib
import json
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parent.parent

CONTAINER = ROOT / "data/main/main.bin"
TWIN = dict(asm=ROOT / "asm/ovl_main_80400000.s", start=0x269000, end=0x2719E0,
            vram=0x80400000, rodata_end=0x804008A0)
LIVE = dict(asm=ROOT / "asm/ovl_main_80020000.s", start=0x25D000, end=0x264E20,
            vram=0x80020000, rodata_end=0x80020924)

MARKER = b'Assertion failed: file "%s", line %d\n\x00'
INSN = re.compile(r"^\s*/\* (\w+) (\w+) (\w+) \*/\s+(\S+)\s*(.*?)\s*$")
GLABEL = re.compile(r"^glabel (\S+)")
HILO = re.compile(r"%(?:hi|lo)\(([\w.]+)\)")
SYM = re.compile(r"(?:D_|jtbl_|func_)([0-9A-Fa-f]{8})")


def filenames(block):
    """{vram_of_string: name} for each assert filename, read from the container.

    Not from the asm: spimdisasm only emits `.asciz` where it is confident and
    emits most of these as `.word`, so the asm is an unreliable source.
    """
    data = CONTAINER.read_bytes()
    out, i = {}, block["start"]
    while True:
        i = data.find(MARKER, i, block["end"])
        if i < 0:
            return out
        j = i + len(MARKER)
        while j % 4:
            j += 1
        name = data[j : data.find(b"\x00", j)].decode("latin1")
        if re.fullmatch(r"[\w./-]{2,40}", name):
            out[block["vram"] + j - block["start"]] = name
        i = j


def fmt_anchors(block, names):
    """[(fmt_string_vram, unit)] in address order: each unit's rodata anchor."""
    data = CONTAINER.read_bytes()
    out, i = [], block["start"]
    while True:
        i = data.find(MARKER, i, block["end"])
        if i < 0:
            break
        j = i + len(MARKER)
        while j % 4:
            j += 1
        v = block["vram"] + j - block["start"]
        if v in names:
            out.append((block["vram"] + i - block["start"], names[v]))
        i = j
    return out


def functions(block):
    """[(vram, name, words, [(op, operands)], {sym_vram})] in address order."""
    out, cur = [], None
    for line in block["asm"].read_text().splitlines():
        m = GLABEL.match(line)
        if m:
            cur = [m.group(1), [], [], set()]
            out.append(cur)
            continue
        m = INSN.match(line)
        if m and cur is not None:
            cur[1].append((int(m.group(2), 16), int(m.group(3), 16)))
            cur[2].append((m.group(4), m.group(5)))
            for s in HILO.findall(m.group(5)):
                mm = SYM.fullmatch(s)
                if mm:
                    cur[3].add(int(mm.group(1), 16))
    return [(w[0][0], n, [x[1] for x in w], ops, syms)
            for n, w, ops, syms in out if w]


def assert_sites(funcs, names):
    """[(func_name, site_vram, unit, line)] for every assert site."""
    sites = []
    for _, fname, words, ops, _ in funcs:
        pend = None
        for k, (op, operand) in enumerate(ops):
            if op == "addiu" and operand.startswith("$a1, $a1, %lo("):
                s = HILO.search(operand)
                mm = SYM.fullmatch(s.group(1)) if s else None
                v = int(mm.group(1), 16) if mm else None
                pend = v if v in names else None
            elif op == "jal" and pend is not None and k + 1 < len(ops):
                dop, dops = ops[k + 1]
                m = re.fullmatch(r"\$a2, \$zero, (0x[0-9A-Fa-f]+|\d+)", dops)
                if dop == "addiu" and m:
                    sites.append((fname, 0, names[pend], int(m.group(1), 0)))
                pend = None
            elif op in ("jal", "jr"):
                pend = None
    return sites


def rodata_units(block, anchors):
    """addr -> unit, for addresses inside the block's own rodata."""
    starts = [a for a, _ in anchors]
    units = [u for _, u in anchors]

    def f(addr):
        if not starts or addr < starts[0] or addr >= block["rodata_end"]:
            return None
        return units[bisect.bisect_right(starts, addr) - 1]

    return f


def mask(w):
    """Drop the fields that differ between the two links."""
    op = w >> 26
    if op in (2, 3):                       # j / jal: opcode only
        return 0x0C000000
    if op in (0x0F, 0x09, 0x23, 0x2B, 0x21, 0x25, 0x29, 0x28, 0x20, 0x24,
              0x2D, 0x0D, 0x0C, 0x0E, 0x0A, 0x0B, 4, 5, 6, 7, 1):
        return w & 0xFFFF0000              # i-type: drop imm16
    return w


def similarity(a, b):
    """How much of the SHORTER body is found, in order, in the other.

    Deliberately not difflib's symmetric `ratio()`. The twin functions that
    carry the most information are the ones holding asserts, and those are
    exactly the ones whose live counterpart is missing a block per assert --
    roughly 8 words each. A symmetric ratio penalises that insertion twice and
    drops precisely the anchors worth propagating: with `ratio()` at 0.5, 24 of
    the twin's 33 anchored functions failed to pair. Normalising the matched
    length by the shorter body treats the extra blocks as insertion, not
    disagreement.
    """
    if not a or not b:
        return 0.0
    lo, hi = min(len(a), len(b)), max(len(a), len(b))
    if lo / hi < 0.3:
        return 0.0
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    if sm.real_quick_ratio() < 0.3:
        return 0.0
    return sum(blk.size for blk in sm.get_matching_blocks()) / lo


def align(live, twin, threshold=0.5):
    """Order-preserving global alignment; [(live_idx, twin_idx)]."""
    A = [[mask(w) for w in f[2]] for f in live]
    B = [[mask(w) for w in f[2]] for f in twin]
    n, m = len(A), len(B)
    prev = array.array("d", [0.0] * (m + 1))
    ptr = []
    for i in range(1, n + 1):
        curr = array.array("d", [0.0] * (m + 1))
        row = bytearray(m + 1)
        ai = A[i - 1]
        for j in range(1, m + 1):
            s = similarity(ai, B[j - 1])
            diag = prev[j - 1] + s if s >= threshold else -1.0
            up, left = prev[j], curr[j - 1]
            if diag >= up and diag >= left:
                curr[j], row[j] = diag, 1
            elif up >= left:
                curr[j], row[j] = up, 2
            else:
                curr[j], row[j] = left, 3
        ptr.append(row)
        prev = curr
    pairs, i, j = [], n, m
    while i > 0 and j > 0:
        d = ptr[i - 1][j]
        if d == 1:
            pairs.append((i - 1, j - 1))
            i -= 1
            j -= 1
        elif d == 2:
            i -= 1
        else:
            j -= 1
    pairs.reverse()
    return pairs


def anchored_units(funcs, sites, ro_unit):
    """idx -> unit, for every function a signal names. Plus the per-unit runs."""
    pos = {f[1]: i for i, f in enumerate(funcs)}
    by_idx = collections.defaultdict(collections.Counter)
    lines = collections.defaultdict(list)
    for fname, _, unit, line in sites:
        by_idx[pos[fname]][unit] += 1
        lines[pos[fname]].append(line)
    for i, f in enumerate(funcs):
        for a in f[4]:
            u = ro_unit(a)
            if u:
                by_idx[i][u] += 1
    conflicts = {i: dict(c) for i, c in by_idx.items() if len(c) > 1}
    assign = {i: c.most_common(1)[0][0] for i, c in by_idx.items()}
    return assign, lines, conflicts


def runs(assign, order):
    """unit -> (first_idx, last_idx, [foreign idxs inside the run])."""
    out = {}
    for unit in dict.fromkeys(assign[i] for i in sorted(assign)):
        idxs = sorted(i for i in assign if assign[i] == unit)
        lo, hi = idxs[0], idxs[-1]
        foreign = [i for i in range(lo, hi + 1) if i in assign
                   and assign[i] != unit]
        out[unit] = (lo, hi, foreign, idxs)
    return out


def report_twin(args):
    names = filenames(TWIN)
    anchors = fmt_anchors(TWIN, names)
    funcs = functions(TWIN)
    sites = assert_sites(funcs, names)
    ro = rodata_units(TWIN, anchors)
    assign, lines, conflicts = anchored_units(funcs, sites, ro)
    r = runs(assign, [f[1] for f in funcs])

    print(f"twin 0x{TWIN['vram']:08X}: {len(funcs)} functions, "
          f"{len(names)} units named, {len(sites)} assert sites, "
          f"{len(assign)} functions anchored by a signal")
    if conflicts:
        print("\nDISAGREEMENT (a function anchored to more than one unit):")
        for i, c in conflicts.items():
            print(f"   idx {i} {funcs[i][1]}: {c}")
    print()
    print(f"{'unit':14s} {'idx run':>12s} {'anch':>5s} {'lines':>13s}  "
          f"{'vram run':>21s}  contiguous")
    prev_hi = -1
    for unit, (lo, hi, foreign, idxs) in r.items():
        ls = [l for i in idxs for l in lines.get(i, [])]
        lr = f"{min(ls)}-{max(ls)}" if ls else "-"
        ok = "yes" if not foreign else f"NO: {foreign}"
        print(f"{unit:14s} {lo:5d}..{hi:<5d} {len(idxs):5d} {lr:>13s}  "
              f"0x{funcs[lo][0]:08X}..0x{funcs[hi][0]:08X}  {ok}")
        if lo - prev_hi > 1:
            print(f"{'':14s} {'':12s}   ^ {lo - prev_hi - 1} unanchored "
                  f"functions before this unit")
        prev_hi = max(prev_hi, hi)
    tail = len(funcs) - 1 - prev_hi
    if tail > 0:
        print(f"{'':14s} {'':12s}   {tail} functions after the last anchored "
              f"unit (PSY-Q library stubs, not game code)")
    if args.sites:
        print()
        for fname, _, unit, line in sites:
            print(f"  {unit:14s} line {line:5d}  {fname}")
    return funcs, assign, lines, r


def report_live(args):
    twin_funcs, twin_assign, twin_lines, _ = report_twin(args)
    live_funcs = functions(LIVE)
    print()
    print(f"live 0x{LIVE['vram']:08X}: {len(live_funcs)} functions. Aligning "
          f"against the twin (masked words, order-preserving)...")
    pairs = align(live_funcs, twin_funcs)
    t2l = {j: i for i, j in pairs}
    print(f"  {len(pairs)} of {len(live_funcs)} live functions pair with a "
          f"twin function")

    # Carry the twin's anchors across, then fill each unit's run by contiguity.
    anchored = {t2l[j]: u for j, u in twin_assign.items() if j in t2l}
    unmapped = sorted(u for j, u in twin_assign.items() if j not in t2l)
    order = sorted(anchored)
    print(f"  {len(anchored)} live functions inherit a unit directly"
          + (f"; {len(unmapped)} twin anchors did not pair" if unmapped else ""))

    print()
    print(f"{'unit':14s} {'live idx run':>14s} {'anch':>5s} "
          f"{'live vram run':>21s}  contiguous")
    r = runs(anchored, [f[1] for f in live_funcs])
    prev_hi = -1
    out_units = {}
    for unit, (lo, hi, foreign, idxs) in r.items():
        ok = "yes" if not foreign else f"NO: {foreign}"
        print(f"{unit:14s} {lo:6d}..{hi:<6d} {len(idxs):5d} "
              f"0x{live_funcs[lo][0]:08X}..0x{live_funcs[hi][0]:08X}  {ok}")
        if lo - prev_hi > 1:
            print(f"{'':14s} {'':14s}   ^ {lo - prev_hi - 1} unanchored before")
        prev_hi = max(prev_hi, hi)
        out_units[unit] = dict(
            first_idx=lo, last_idx=hi, anchors=len(idxs),
            first_func=live_funcs[lo][1], last_func=live_funcs[hi][1],
            first_vram=live_funcs[lo][0], last_vram=live_funcs[hi][0],
            foreign=foreign)
    tail = len(live_funcs) - 1 - prev_hi
    if tail > 0:
        print(f"{'':14s} {'':14s}   {tail} functions after the last unit")

    # Boundary brackets. A unit whose twin anchor did not pair still has its
    # live position constrained, because the alignment is monotonic: the
    # anchor sits between the nearest paired twin indices below and above it,
    # so its live counterpart sits between their live partners. This is what
    # turns "unanchored before" into a stated interval, and it is the only way
    # cardbk.c -- whose every anchor is an assert-bearing function, none of
    # which paired -- gets placed at all.
    J = sorted(t2l)

    def bracket(j):
        lo = next((t2l[x] for x in reversed(J) if x <= j), -1)
        hi = next((t2l[x] for x in J if x >= j), len(live_funcs))
        return lo, hi

    twin_runs = runs(twin_assign, [f[1] for f in twin_funcs])
    print()
    print("THE MAP. Per unit: where its first and last ANCHORED function sits "
          "in the live block (an interval when that anchor did not pair), and "
          "the interval the unit's own START boundary must lie in -- after the "
          "previous unit's last anchor, at or before this unit's first anchor.")
    print(f"{'unit':14s} {'starts in':>14s} {'first anchor':>14s} "
          f"{'last anchor':>14s} {'twin lines':>12s} {'paired':>7s}")
    rowsout = []
    prev_last_hi = -1
    for unit, (tlo, thi, _, tidxs) in twin_runs.items():
        flo, fhi = bracket(tlo)
        llo, lhi = bracket(thi)
        ls = [l for i in tidxs for l in twin_lines.get(i, [])]
        lr = f"{min(ls)}-{max(ls)}" if ls else "-"
        npair = sum(1 for i in tidxs if i in t2l)
        start_lo, start_hi = prev_last_hi + 1, fhi
        # The two brackets are derived independently and can overlap by an
        # index; intersect so the row cannot contradict itself.
        flo = max(flo, start_lo)
        f_s = f"{flo}" if flo == fhi else f"[{flo}, {fhi}]"
        l_s = f"{llo}" if llo == lhi else f"[{llo}, {lhi}]"
        s_s = (f"{start_lo}" if start_lo == start_hi
               else f"[{start_lo}, {start_hi}]")
        print(f"{unit:14s} {s_s:>14s} {f_s:>14s} {l_s:>14s} {lr:>12s} "
              f"{npair:2d}/{len(tidxs):<2d}")
        rowsout.append(dict(unit=unit, starts_in=[start_lo, start_hi],
                            first_anchor=[flo, fhi], last_anchor=[llo, lhi],
                            twin_lines=[min(ls), max(ls)] if ls else None,
                            anchors_paired=npair, anchors=len(tidxs)))
        prev_last_hi = max(prev_last_hi, lhi)
    bad = [(a["unit"], b["unit"]) for a, b in zip(rowsout, rowsout[1:])
           if a["last_anchor"][0] > b["first_anchor"][1]]
    print()
    if bad:
        print("ORDER VIOLATION: these units overlap in the live block, so the "
              "alignment or an anchor is wrong: " + str(bad))
    else:
        print("All nine units stay in order and non-overlapping in the live "
              "block, which the alignment did not enforce -- it is a check it "
              "passed.")
    print("Source files are at least as long as their twin line maxima, an "
          "independent check on any proposed split.")

    # The payoff: the live block's own rodata, partitioned by the unit of the
    # functions that reach it. This is what a per-TU rodata split needs.
    print()
    print("Live rodata partitioned by the unit of its referencing functions "
          "(what a per-TU rodata split needs):")
    ro_owner = collections.defaultdict(collections.Counter)
    for i, f in enumerate(live_funcs):
        u = anchored.get(i)
        if not u:
            continue
        for a in f[4]:
            if LIVE["vram"] <= a < LIVE["rodata_end"]:
                ro_owner[a][u] += 1
    disputed = {a: dict(c) for a, c in ro_owner.items() if len(c) > 1}
    for a in sorted(ro_owner):
        c = ro_owner[a]
        mark = "" if len(c) == 1 else "   DISPUTED " + str(dict(c))
        print(f"   0x{a:08X}  {c.most_common(1)[0][0]:14s}{mark}")
    if not ro_owner:
        print("   (none: no anchored live function reaches the block's rodata)")
    elif not disputed:
        print(f"   {len(ro_owner)} addresses, none disputed.")

    if args.json:
        pathlib.Path(args.json).write_text(json.dumps(dict(
            boundaries=rowsout,
            live_units=out_units,
            live_function_unit={live_funcs[i][1]: u
                                for i, u in sorted(anchored.items())},
            live_rodata_owner={f"0x{a:08X}": c.most_common(1)[0][0]
                               for a, c in sorted(ro_owner.items())},
            pairs=[[live_funcs[i][1], twin_funcs[j][1]] for i, j in pairs],
        ), indent=1))
        print(f"\nwrote {args.json}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--live", action="store_true",
                    help="propagate the map onto the live 0x80020000 overlay")
    ap.add_argument("--sites", action="store_true", help="list assert sites")
    ap.add_argument("--json", metavar="PATH")
    args = ap.parse_args()
    if args.live:
        report_live(args)
    else:
        report_twin(args)


if __name__ == "__main__":
    main()
