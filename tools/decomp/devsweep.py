#!/usr/bin/env python3
"""devsweep.py FUNC [--rounds N] [--body PATH] -- sweep the two FREE-REFERENCE
WRAPPERS over every statement: `do { s } while (0);` and the duplicated-arm
`if (v) { s } else { s }` for every local v.

    python3 tools/decomp/devsweep.py func_800B45E0

Both wrappers cost NOTHING and are ordinary C: flow.c:2128 weights each
reference by `loop_depth`, so a do-while at depth 1 counts every reference
inside it twice, and jump2's cross-jumping merges two identical arms back into
one copy of the statement.  Most of this block's parks were brought to their
current state by exactly this sweep, run by hand out of a scratchpad script.

THE REASON IT IS A TOOL NOW IS THAT A SWEEP'S NEGATIVE GOES STALE.  Every one
of those park notes is scoped to the body it was swept on, and the bodies have
all changed since: a width, a cancelled pair, a moved statement.
func_800A05A4 is the proof -- 210 entry-block orders had been swept to
exhaustion and the conclusion held until a `do { } while (0);` landed, after
which one of the orders that had measured 59 measured 45.  Re-run this after
landing anything.

The duplicated-arm device needs a CONDITION, and the sweep uses each local in
turn.  A local that is not yet initialised at that point reads as
indeterminate, which the compiler does not care about -- both arms are
identical, so the branch cannot change what the function computes -- but it is
worth preferring a condition that is live and defined when two spellings
measure the same.  See docs/matching-levers.md.
"""
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
from onefunc import find_definition
import parkresidue as pr

TOOL = "devsweep"
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
    sp = os.path.join(scratch, "devsweep", func)
    os.makedirs(sp, exist_ok=True)
    unit_path, unit = pr.unit_of(func)
    dst = os.path.join(sp, os.path.basename(unit_path))
    subprocess.run(["cp", unit_path, dst], check=True)
    r = subprocess.run([sys.executable, os.path.join(HERE, "parksplice.py"),
                        func, dst], capture_output=True, text=True)
    if "spliced" not in r.stdout:
        raise SystemExit("devsweep: parksplice failed:\n" + r.stdout + r.stderr)

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

    # Sweep on onefunc.py's standalone when the unit is big enough for 25 s a
    # candidate to matter -- but only after it reproduces the unit's residue,
    # which is the protocol onefunc's docstring insists on.
    measure = measure_unit
    if len(base_txt.split("\n")) > 8000:
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
    cur, best, applied = fn, (rows, w), []
    for rnd in range(rounds):
        lines = cur.split("\n")
        cands = candidates(cur)
        if not cands:
            print("devsweep: no locals found for the duplicated-arm device")
        # Every SINGLE-LINE statement is wrappable; a multi-line one would
        # need brace matching and the sweep has never needed it.
        stmts = [i for i in positions(lines, 0)
                 if lines[i].strip().endswith(";")
                 and not DECL.match(lines[i])
                 and "while (0)" not in lines[i]]
        if rnd == 0:
            print("%s: %d statement(s), %d name(s) -> %d candidates per round"
                  % (func, len(stmts), len(cands), len(stmts) * (1 + len(cands))))
        found = None
        for i in stmts:
            ind = re.match(r"\s*", lines[i]).group(0)
            tries = [([ind + "do", ind + "{", "    " + lines[i], ind + "} while (0);"],
                      "do-while around `%s'" % lines[i].strip()[:44])]
            for v, lo, hi in cands:
                if not (lo <= i <= hi):
                    continue
                tries.append(([ind + "if (" + v + ")", ind + "{", "    " + lines[i],
                               ind + "}", ind + "else", ind + "{", "    " + lines[i],
                               ind + "}"],
                              "if (%s) duplicated arm around `%s'"
                              % (v, lines[i].strip()[:30])))
            for repl, what in tries:
                cand = lines[:i] + repl + lines[i + 1:]
                body = "\n".join(cand)
                rr, ww, _ = measure(body)
                if ww == tw and (rr, ww) < best:
                    best = (rr, ww)
                    found = (body, what)
                    print("  round %d: %d rows, %d words  %s" % (rnd, rr, ww, what))
        if not found:
            break
        cur = found[0]
        applied.append(found[1])

    if applied and measure is not measure_unit:
        vr, vw, _ = measure_unit(cur)
        print("verified against the real unit: %d rows, %d/%d words" % (vr, vw, tw))
        if (vr, vw) != best:
            print("devsweep: *** STANDALONE AND UNIT DISAGREE (%d/%d vs %d/%d);"
                  " trust the unit." % (vr, vw, best[0], best[1]))
            best = (vr, vw)
    print("final %d rows, %d/%d words" % (best[0], best[1], tw))
    if applied:
        out = os.path.join(sp, "best.c")
        open(out, "w").write(cur)
        print("devices: " + " | ".join(applied))
        print("saved: " + out)
    else:
        print("no wrapper helps")


if __name__ == "__main__":
    main()
