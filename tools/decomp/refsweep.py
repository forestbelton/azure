#!/usr/bin/env python3
"""refsweep.py FUNC [--rounds N] -- insert a CANCELLED INCREMENT PAIR
(`v++; v--;`) for every local at every statement position, and report what
improves.

    python3 tools/decomp/refsweep.py func_800A94A0

WHY THIS EXISTS.  On 2026-11-21 decomp-permuter closed 18 of func_800A94A0's
59 rows with two statements:

    n = func_8003FD64(0x12, D_80083498);
    n++;
    n--;

The pair compiles to NOTHING -- combine cancels the `addiu $x, 0x58` against
the `addiu $x, -0x58` -- but it is four more references to `n` in flow.c's
count, and `allocno_compare` orders by floor_log2(n_refs) * n_refs /
live_length, so it moves n up the allocation queue and permutes every
callee-saved register below it.  `n += 1; n -= 1;` and `n = &n[1];
n = &n[-1];` are the same three spellings of the same cancelled pair and
measure identically.

IT IS A DIFFERENT LEVER FROM `do { } while (0);`, which is what this repo had
been sweeping.  The do-while weights the references INSIDE it by loop_depth;
the cancelled pair adds references to ONE NAMED LOCAL at a point of your
choosing, without wrapping any statement.  On func_800A94A0 the do-while
around the same call is 66 rows -- SEVEN WORSE than the park -- because the
only reference it weights is the assignment, which is not where the weight was
wanted.  The two levers are not substitutes and a park wants both swept.

WHAT IT WILL AND WILL NOT TRY.  Only locals it can legally increment: the
integer scalars and every pointer.  Only positions where a statement is legal:
after the declaration block (C89 -- a statement before a declaration is an
error, not a worse score), at a statement boundary, and never before an `else`
or a `} while`.  A candidate that fails to compile scores 999 and is dropped,
so an illegal insertion costs a measurement and nothing else.

THE PAIR IS NOT AN ASM DEVICE and it is ordinary C, but it is still a device:
it has no reason to be in the source beyond the register assignment it buys.
Land it with a comment saying so, the way the parks do.
"""
TAG = "refsweep"
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
from onefunc import find_definition
import parkresidue as pr

# LINE-BUFFER STDOUT. A sweep's findings are printed as it goes, but Python
# block-buffers stdout when it is a file, so a run killed by `timeout' loses
# everything still in the 8 KB buffer -- and these runs are long enough that
# being killed is the NORMAL ending. On 2026-11-22 a 90-minute refsweep of
# func_800ADE74 under `timeout' produced AN EMPTY LOG: SIGTERM discarded the
# whole buffer, including every improvement it had found. The agent worked
# around it with `python3 -u', which is correct but has to be remembered;
# this makes it the default so a killed sweep still leaves its finds on disk.
try:
    sys.stdout.reconfigure(line_buffering=True)
except Exception:
    pass


TOOL = "refsweep"
DECL = re.compile(r"^(\s*)((?:const\s+)?[A-Za-z_][A-Za-z0-9_]*)\s+(\**)"
                  r"([A-Za-z_][A-Za-z0-9_]*)\s*(\[[^\]]*\])?\s*(=[^;]*)?;$")
SCALAR = {"s8", "u8", "s16", "u16", "s32", "u32", "int", "long", "short",
          "char", "unsigned", "signed"}


def scope_of(lines, i):
    """(first, last) line indices where a declaration on line i is in scope."""
    depth = 0
    for j in range(i):
        s = lines[j].strip()
        depth += s.count("{") - s.count("}")
    d, j = depth, i + 1
    while j < len(lines):
        s = lines[j].strip()
        d += s.count("{") - s.count("}")
        if d < depth:
            return i + 1, j
        j += 1
    return i + 1, len(lines) - 1


def candidates(body):
    """[(name, first, last)] for every local or parameter we may increment,
    with the line range over which it is in scope.

    PARAMETERS AND INNER-BLOCK DECLARATIONS COUNT.  The first version of this
    tool read only the function's top declaration block, and so swept 3 of
    func_8009A3D0's 9 names -- missing the parameter `f', which is the one
    name that park's whole residue is about.  A park that declares its
    working set inside the arm that uses it (which is most of the good ones)
    was almost entirely unswept.
    """
    lines = body.split("\n")
    out = []
    # the parameter list: everything between the first ( and its matching )
    head = body[:body.index("{")]
    inner = head[head.index("(") + 1:head.rindex(")")]
    first_stmt = next((k for k, l in enumerate(lines) if l.strip() == "{"), 0) + 1
    for part in inner.split(","):
        part = part.strip()
        m = re.match(r"^((?:const\s+|unsigned\s+|signed\s+)?[A-Za-z_][A-Za-z0-9_]*)"
                     r"\s*(\**)\s*([A-Za-z_][A-Za-z0-9_]*)$", part)
        if m and (m.group(2) or m.group(1) in SCALAR):
            out.append((m.group(3), first_stmt, len(lines) - 1))
    for i, l in enumerate(lines):
        m = DECL.match(l)
        if not m:
            continue
        ty, stars, name, arr = m.group(2), m.group(3), m.group(4), m.group(5)
        if arr:
            continue                      # an array name is not an lvalue
        if "__asm__" in l or "register" in l:
            continue                      # a hard-register device is not ours
        if not (stars or ty in SCALAR):
            continue
        lo, hi = scope_of(lines, i)
        out.append((name, lo, hi))
    return out


def positions(lines, first, last=None):
    """Statement boundaries in [first, last] where `v++;` is legal."""
    out = []
    for i in range(first, len(lines) if last is None else last + 1):
        s = lines[i].strip()
        if not s:
            continue
        if s.startswith("else") or s.startswith("} while") or s.startswith("}while"):
            continue
        prev = ""
        for j in range(i - 1, -1, -1):
            if lines[j].strip():
                prev = lines[j].strip()
                break
        # A statement may only start where the previous one ENDED: a line that
        # closes with `)' or `,' is the middle of a multi-line expression, and
        # inserting there is a syntax error rather than a worse score.
        if prev and prev[-1] not in ";{}:":
            continue
        out.append(i)
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
    sp = os.path.join(scratch, "refsweep", func)
    os.makedirs(sp, exist_ok=True)
    unit_path, unit = pr.unit_of(func)
    dst = os.path.join(sp, os.path.basename(unit_path))
    subprocess.run(["cp", unit_path, dst], check=True)
    r = subprocess.run([sys.executable, os.path.join(HERE, "parksplice.py"),
                        func, dst], capture_output=True, text=True)
    if "spliced" not in r.stdout:
        raise SystemExit("refsweep: parksplice failed:\n" + r.stdout + r.stderr)

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

    # Sweep on onefunc.py's standalone whenever the unit is big enough for the
    # per-candidate cost to matter -- but only after it reproduces the unit's
    # residue, which is the protocol onefunc's docstring insists on and the
    # reason the check below is not optional.
    #
    # THE THRESHOLD WAS 8000 LINES AND THE CARVE MADE THAT WRONG. The dungeon
    # block is six translation units now and FIVE OF THE SIX are under 8000
    # lines (1029, 4489, 4815, 5781, 6437; only csrc/ovl_dungeon.c at 26495 is
    # over), so no sweep on a carved unit has built a standalone since the
    # carve -- measuring the whole unit at ~0.23 s a candidate instead of
    # ~0.09 s, over thousands of insertion points. The c2824e agent lost two
    # 45-minute refsweep runs to it. The standalone costs one build, the
    # saving is per candidate and sweeps measure hundreds to thousands, so the
    # break-even is far below any unit we have; and a standalone that does not
    # reproduce the residue is DISCARDED a few lines down, so a low threshold
    # cannot cause a wrong measurement, only a wasted build.
    measure = measure_unit
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
        cands = candidates(cur)
        if not cands:
            print("refsweep: no incrementable locals found")
            return
        total = 0
        plans = []
        for v, lo, hi in cands:
            pos = positions(lines, lo, hi)
            total += len(pos)
            plans.append((v, pos))
        if rnd == 0:
            print("%s: %d name(s), %d candidate insertion point(s)"
                  % (func, len(cands), total))
        found = None
        for v, pos in plans:
            for i in pos:
                ind = re.match(r"\s*", lines[i]).group(0)
                cand = lines[:i] + [ind + v + "++;", ind + v + "--;"] + lines[i:]
                body = "\n".join(cand)
                rr, ww, _ = measure(body)
                if key(rr, ww) < best_key:
                    best, best_key = (rr, ww), key(rr, ww)
                    found = (body, "%s++/-- before `%s'" % (v, lines[i].strip()[:48]))
                    print("  round %d: %d rows, %d words  %s" % (rnd, rr, ww, found[1]))
        if not found:
            break
        cur = found[0]
        applied.append(found[1])

    if applied and measure is not measure_unit:
        vr, vw, _ = measure_unit(cur)
        print("verified against the real unit: %d rows, %d/%d words" % (vr, vw, tw))
        if (vr, vw) != best:
            print("refsweep: *** STANDALONE AND UNIT DISAGREE (%d/%d vs %d/%d);"
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
        print("devices: " + " | ".join(applied))
        print("saved: " + out)
    else:
        print("no cancelled pair helps")


if __name__ == "__main__":
    main()
