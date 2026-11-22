#!/usr/bin/env python3
"""stmtsweep.py FUNC [--rounds N] -- move each SIMPLE ASSIGNMENT to every other
legal position in its own block, and report what improves.

    python3 tools/decomp/stmtsweep.py func_800A05A4

WHY THIS EXISTS.  On 2026-11-21 decomp-permuter closed two of func_800A05A4's
47 rows with exactly one change: `r = NULL;` moved from the middle of the
entry block to the function's FIRST statement.  Nothing else.  DECL_RTL is
created lazily by expand_expr, so a local's pseudo number follows its FIRST
USE and not its declaration -- which is why declaration order is not a lever
(446 measurements, docs/matching-levers.md) and statement order IS one.

AND IT IS WHY A SWEEP'S NEGATIVE GOES STALE.  That same park had already
swept all 210 interleavings of its entry block and concluded order "cannot get
below 59".  True of the body as it then stood; the `do { } while (0);` device
landed afterwards changed which pseudo wins the contested register, and one of
the orders that had scored 59 became 45.  Re-run this after landing any
device.

WHAT IT MOVES, AND WHY ONLY THAT.  A statement is a candidate only when it is
`name = <expr>;` with `name` a local scalar or pointer, and `<expr>` built
from locals, parameters and constants -- no call, no dereference, no `[`, no
`->`.  It is moved only within its own block, and only across statements that
MENTION NEITHER the target nor any name in the expression.  Those rules make a move over
LOCALS semantics-preserving by inspection: nothing aliases a local whose
address is never taken (a local with `&name` anywhere in the body is
excluded), and a crossed statement that does not mention a name can neither
read nor write it.

A GLOBAL IN THE EXPRESSION IS DIFFERENT and the tool says so instead of
pretending otherwise.  `b = D_800E3648;` on an `extern u8 D_800E3648[]` is an
address constant -- array-to-pointer decay, no memory read -- and moves freely.
`f = D_800814A0 | 0x8000;` on an `extern u32 D_800814A0` is a real load, and
moving it across a call or a store through a pointer is only safe if neither
can reach that address.  Textual non-mention does not prove that: a call can
write any global, and gcc 2.7.2 has no type-based alias analysis, so it
reloads.  Such a move is printed with `CHECK ALIASING' and is yours to
justify.  Usually the target itself justifies it -- if the target's own
instruction order has the load before the store, that IS the order the
original source had.

So it will not find the reorderings that are only *probably* safe -- hoisting
a load out of a short-circuit, sinking a store past a call.  decomp-permuter
will offer those, and two of its finds have been rejected on exactly that
ground (func_800AD6FC made a conditional volatile read unconditional,
func_8009E798 shifted a colour channel before the store that initialises it).
A tool that only proposes safe moves is the right division of labour.
"""
TAG = "stmtsweep"
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
from onefunc import find_definition
import parkresidue as pr

DECL = re.compile(r"^(\s*)((?:const\s+)?[A-Za-z_][A-Za-z0-9_]*)\s+(\**)"
                  r"([A-Za-z_][A-Za-z0-9_]*)\s*(\[[^\]]*\])?\s*(=[^;]*)?;$")
ASSIGN = re.compile(r"^(\s*)([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([^=][^;]*);$")
TOOL = "stmtsweep"
_ARRAYS = None


def ARRAY_RE(name):
    """True when `name' is declared as an array anywhere in csrc/, so that
    using it is an address constant and not a load."""
    global _ARRAYS
    if _ARRAYS is None:
        _ARRAYS = set()
        d = os.path.join(os.path.dirname(os.path.dirname(HERE)), "csrc")
        for fn in os.listdir(d) if os.path.isdir(d) else []:
            if not fn.endswith((".c", ".h")):
                continue
            with open(os.path.join(d, fn), errors="ignore") as fh:
                for m in re.finditer(r"\bextern\s+[A-Za-z_][A-Za-z0-9_]*\s*\**\s*"
                                     r"([A-Za-z_][A-Za-z0-9_]*)\s*\[", fh.read()):
                    _ARRAYS.add(m.group(1))
    return name in _ARRAYS
NAME = re.compile(r"\b[A-Za-z_][A-Za-z0-9_]*\b")
BAD_IN_EXPR = ("(", "[", "*", "->", ".")


def local_names(lines):
    names, last = set(), -1
    for i, l in enumerate(lines):
        m = DECL.match(l)
        if not m:
            if names and l.strip() and not l.strip().startswith("/*"):
                break
            continue
        last = i
        names.add(m.group(4))
    return names, last


def blocks(lines, first):
    """Map each line index to its brace depth, so a move stays in one block."""
    depth, out = 0, {}
    for i, l in enumerate(lines):
        s = l.strip()
        opens, closes = s.count("{"), s.count("}")
        d = depth - (closes if s.startswith("}") else 0)
        out[i] = d
        depth += opens - closes
    return out


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    if not args:
        raise SystemExit((__doc__ or "").strip().split("\n\n")[0])
    func = args[0]
    rounds = 3
    for i, a in enumerate(sys.argv):
        if a == "--rounds":
            rounds = int(sys.argv[i + 1])

    scratch = os.environ.get("AZURE_SCRATCH") or "/tmp"
    sp = os.path.join(scratch, "stmtsweep", func)
    os.makedirs(sp, exist_ok=True)
    unit_path, unit = pr.unit_of(func)
    dst = os.path.join(sp, os.path.basename(unit_path))
    subprocess.run(["cp", unit_path, dst], check=True)
    r = subprocess.run([sys.executable, os.path.join(HERE, "parksplice.py"),
                        func, dst], capture_output=True, text=True)
    if "spliced" not in r.stdout:
        raise SystemExit("stmtsweep: parksplice failed:\n" + r.stdout + r.stderr)

    base_txt = open(dst).read()
    st, en = find_definition(base_txt, func)
    head, fn, tail = base_txt[:st], base_txt[st:en], base_txt[en:]

    # `--body PATH' sweeps the definition in PATH instead of the park's own.
    # It is how a sweep is checked against a case whose answer is known (the
    # park BEFORE the change the permuter found), and how a candidate that is
    # not the park yet gets swept without editing docs/dungeon-parks.
    for _i, _a in enumerate(sys.argv):
        if _a == "--body":
            _t = open(sys.argv[_i + 1]).read()
            _s = find_definition(_t, func)
            if not _s:
                raise SystemExit("%s: no definition of %s in %s"
                                 % (TOOL, func, sys.argv[_i + 1]))
            fn = _t[_s[0]:_s[1]]
            base_txt = head + fn + tail
            open(dst, "w").write(base_txt)

    def measure_on(path, pre, post):
        def go(body):
            open(path, "w").write(pre + body + post)
            rr = subprocess.run([sys.executable, os.path.join(HERE, "aligndiff.py"),
                                 path, func], capture_output=True, text=True)
            m = re.match(r"built (\d+) words, target (\d+) words, (\d+)",
                         (rr.stdout.strip() or "").split("\n")[0])
            return (int(m.group(3)), int(m.group(1)), int(m.group(2))) if m else (999, 0, 1)
        return go

    measure_unit = measure_on(dst, head, tail)
    rows, w, tw = measure_unit(fn)
    measure = measure_unit
    # 800, not 8000: five of the six carved dungeon units are under 8000
    # lines, so the old threshold silently disabled the standalone for all
    # of them. See refsweep.py for the full reasoning and the cost numbers.
    if len(base_txt.split("\n")) > 800:
        subprocess.run([sys.executable, os.path.join(HERE, "onefunc.py"), func,
                        "--src", dst, "--out", os.path.join(sp, "one")],
                       capture_output=True, text=True)
        one = os.path.join(sp, "one", os.path.basename(unit_path))
        if os.path.exists(one):
            otxt = open(one).read()
            ost, oen = find_definition(otxt, func)
            m1 = measure_on(one, otxt[:ost], otxt[oen:])
            orows, ow, _ = m1(fn)
            if (orows, ow) == (rows, w):
                print("%s: standalone agrees (%d rows, %d words) -- sweeping there"
                      % (func, orows, ow))
                measure = m1
            else:
                print("%s: standalone measures %d/%d against the unit's %d/%d --"
                      " NOT using it" % (orows, ow, rows, w))

    print("%s: base %d rows, %d/%d words" % (func, rows, w, tw))
    # ORDER BY WORD GAP FIRST, THEN ROWS -- and never gate on `ww == tw'.
    #
    # BUT KNOW EXACTLY WHAT THE OLD GATE BROKE, because on 2026-11-22 the
    # scare was worse than the bug and cost three agents a redo instruction.
    # The old gate was `if ww == tw and (rr, ww) < best' with `best' starting
    # at `(rows, w)'.  WHEN THE PARK IS WORD-EXACT (w == tw) THAT IS EXACTLY
    # EQUIVALENT TO THE KEY BELOW: a candidate with ww == tw and rr < rows
    # satisfies (rr, tw) < (rows, tw) and is accepted, and the two predicates
    # enumerated over a grid of candidate (rows, words) disagree ZERO times.
    # So a clean sweep recorded on a word-exact park BEFORE this fix is still
    # valid evidence -- ten of the eleven dungeon parks were word-exact when
    # this was checked, so the fix invalidated almost nothing anyone recorded.
    #
    # It bit a park whose words DIFFER from the target: the old gate demanded
    # ww == tw exactly, so it threw away a partial closing of the gap, and it
    # threw away a candidate that CLOSES the gap while costing rows -- which,
    # since we order word gap before rows, is the candidate we most want.
    # func_800ADE74 at 1826/1817 was that park, and there two of these tools
    # had to be re-implemented by hand before they would report anything.
    #
    # The transferable rule is the one the stale-body check already makes: a
    # sweep is evidence about THE BODY IT RAN ON, not about the function.
    # A strict word-exactness gate is useless on a park that is not already
    # word-exact, and WORSE THAN USELESS on one that is long: the only
    # candidates that pass are the ones reaching the target by DELETING code
    # the target has (shorten_compare will fold a guard away for you).  On
    # func_800ADE74 at 1821 of 1817 words that gate made these tools accept
    # nothing but block deletions, and an agent had to re-implement both.
    # Same fix, same reason, as widthsweep.py and measure_finds.py.
    def key(r, ww):
        return (abs(ww - tw), r)

    cur, best, applied = fn, (rows, w), []
    best_key = key(rows, w)
    for rnd in range(rounds):
        lines = cur.split("\n")
        names, lastdecl = local_names(lines)
        addr_taken = set(m for m in NAME.findall(cur)
                         if ("&" + m) in cur.replace("&&", "")) & names
        depth = blocks(lines, lastdecl + 1)
        cands = []
        for i in range(lastdecl + 1, len(lines)):
            m = ASSIGN.match(lines[i])
            if not m:
                continue
            tgt, expr = m.group(2), m.group(3)
            if tgt not in names or tgt in addr_taken:
                continue
            if any(b in expr for b in BAD_IN_EXPR):
                continue
            ops = set(NAME.findall(expr))
            if ops & addr_taken:
                continue
            # An operand that is neither a local nor an array name (an address
            # constant) is a real load from a global, and crossing a call or a
            # store is then an aliasing question rather than a free move.
            globs = set(o for o in ops if o not in names and not ARRAY_RE(o))
            cands.append((i, tgt, ops | {tgt}, globs))
        if rnd == 0:
            print("%s: %d movable assignment(s)" % (func, len(cands)))
        found = None
        for i, tgt, touched, globs in cands:
            # How far can it travel without crossing a statement that mentions
            # any name it touches?  That bound is what makes the move safe.
            lo = i
            while lo - 1 > lastdecl and depth.get(lo - 1) == depth[i] \
                    and not (set(NAME.findall(lines[lo - 1])) & touched) \
                    and lines[lo - 1].strip() and "{" not in lines[lo - 1] \
                    and "}" not in lines[lo - 1]:
                lo -= 1
            hi = i
            while hi + 1 < len(lines) and depth.get(hi + 1) == depth[i] \
                    and not (set(NAME.findall(lines[hi + 1])) & touched) \
                    and lines[hi + 1].strip() and "{" not in lines[hi + 1] \
                    and "}" not in lines[hi + 1]:
                hi += 1
            for j in range(lo, hi + 1):
                if j == i:
                    continue
                rest = lines[:i] + lines[i + 1:]
                k = j if j < i else j - 1
                cand = rest[:k] + [lines[i]] + rest[k:]
                body = "\n".join(cand)
                rr, ww, _ = measure(body)
                if key(rr, ww) < best_key:
                    best, best_key = (rr, ww), key(rr, ww)
                    crossed = lines[min(i, j):max(i, j) + 1]
                    risky = globs and any("(" in c or "=" in c for c in crossed
                                          if c is not lines[i])
                    found = (body, "`%s' moved %+d line(s)%s"
                             % (lines[i].strip()[:40], j - i,
                                "   *** CHECK ALIASING: crosses a call or store"
                                " and reads %s" % ", ".join(sorted(globs))
                                if risky else ""))
                    print("  round %d: %d rows, %d words  %s" % (rnd, rr, ww, found[1]))
        if not found:
            break
        cur = found[0]
        applied.append(found[1])

    if applied and measure is not measure_unit:
        vr, vw, _ = measure_unit(cur)
        print("verified against the real unit: %d rows, %d/%d words" % (vr, vw, tw))
        if (vr, vw) != best:
            print("stmtsweep: *** STANDALONE AND UNIT DISAGREE (%d/%d vs %d/%d);"
                  " trust the unit." % (vr, vw, best[0], best[1]))
            best, best_key = (vr, vw), key(vr, vw)
    print("final %d rows, %d/%d words" % (best[0], best[1], tw))
    if applied and best[1] < w:
        print("%s: *** THE RESULT IS SHORTER THAN THE BODY YOU STARTED WITH" % TAG)
        print("%s: *** (%d -> %d words).  Check aligndiff for a CONTIGUOUS RUN" % (TAG, w, best[1]))
        print("%s: *** of missing target words -- four or more is a deleted" % TAG)
        print("%s: *** basic block, not a closed word gap." % TAG)
    if applied:
        out = os.path.join(sp, "best.c")
        open(out, "w").write(cur)
        print("moves: " + " | ".join(applied))
        print("saved: " + out)
    else:
        print("no safe move helps")


if __name__ == "__main__":
    main()
