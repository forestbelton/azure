"""constsweep.py FUNC [--rounds N] [--min-uses K] -- route an integer LITERAL
through a local and measure, for every literal in a parked function.

    python3 tools/decomp/constsweep.py func_80089AA0

WHY. A literal operand gets folded into the instruction that uses it -- into a
`lw' displacement, an `addiu' immediate, an `ori'. Through a local it has to
land in a register first, and sometimes that is exactly what the target did.
Twice on 2026-11-21:

    func_8009F644   qo = 0xC, then *(u8 **)(g + qo)      THE MISSING WORD
                    (with the literal, 0xC folds into the lw displacement)
    func_80089AA0   one = 1, then twelve uses of `one'   81 rows -> 75

Both were decomp-permuter finds. This tries the same transform directly.

WHAT IT TRIES, per literal: declare a local, assign it once, and replace the
literal at every use from the assignment onward. The assignment goes before
the first use, and then -- because POSITION MATTERS TO WITHIN ONE STATEMENT --
before each of the next few statements as well. On func_80089AA0 the
difference between the right statement and the one above it was 75 rows
against 79.

It only ever replaces a literal in a STATEMENT, never in a declaration, a
case label, an array bound or a `#' line, and it rejects any candidate whose
word count stops matching the target. Like widthsweep.py it sweeps
onefunc.py's standalone on a large unit, after confirming the standalone
reproduces the unit's own residue.

A WORD OF CAUTION ON WHAT THESE FINDS MEAN. `one = 1' is a scaffold until
something explains it; the project's rule is that a match is a match
(docs/matching-levers.md), but write down in the park what the sweep found and
where the assignment has to sit, because the placement is not recoverable by
re-reading the body.
"""
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
from onefunc import find_definition
import parkresidue as pr


def statement_lines(lines):
    """Indices of lines that are plain statements -- the only safe hosts."""
    out = []
    for i, l in enumerate(lines):
        s = l.strip()
        if not s or s in ("{", "}") or s.startswith(("#", "/*", "*", "//")):
            continue
        if re.match(r"^(case|default|typedef|struct|union|enum)\b", s):
            continue
        if re.match(r"^(s8|u8|s16|u16|s32|u32|void|register|const)\b[^(]*;$", s):
            continue
        out.append(i)
    return out


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    if not args:
        raise SystemExit((__doc__ or "").strip().split("\n\n")[0])
    func = args[0]
    rounds, min_uses, span = 2, 2, 3
    for i, a in enumerate(sys.argv):
        if a == "--rounds":
            rounds = int(sys.argv[i + 1])
        if a == "--min-uses":
            min_uses = int(sys.argv[i + 1])
        if a == "--span":
            span = int(sys.argv[i + 1])

    scratch = os.environ.get("AZURE_SCRATCH") or "/tmp"
    sp = os.path.join(scratch, "constsweep", func)
    os.makedirs(sp, exist_ok=True)
    unit_path, unit = pr.unit_of(func)
    dst = os.path.join(sp, os.path.basename(unit_path))
    subprocess.run(["cp", unit_path, dst], check=True)
    r = subprocess.run([sys.executable, os.path.join(HERE, "parksplice.py"),
                        func, dst], capture_output=True, text=True)
    if "spliced" not in r.stdout:
        raise SystemExit("constsweep: parksplice failed:\n" + r.stdout + r.stderr)

    base_txt = open(dst).read()
    st, en = find_definition(base_txt, func)
    head, fn, tail = base_txt[:st], base_txt[st:en], base_txt[en:]

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
    print("%s: base %d rows, %d/%d words" % (func, rows, w, tw))
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
            if m1(fn)[:2] == (rows, w):
                print("%s: standalone agrees -- sweeping there" % func)
                measure = m1

    cur, best, applied, tag = fn, (rows, w), [], 0
    for rnd in range(rounds):
        lines = cur.split("\n")
        hosts = statement_lines(lines)
        counts = {}
        for i in hosts:
            for lit in re.findall(r"\b(0x[0-9A-Fa-f]+|\d+)\b", lines[i]):
                counts.setdefault(lit, []).append(i)
        cands = [(lit, ix) for lit, ix in counts.items()
                 if len(ix) >= min_uses and lit not in ("0",)]
        cands.sort(key=lambda kv: -len(kv[1]))
        found = None
        for lit, ix in cands[:12]:
            name = "kk%d" % tag
            for shift in range(span):
                at = ix[0] + shift
                if at not in hosts and shift:
                    continue
                new = lines[:]
                for i in ix:
                    if i >= at:
                        new[i] = re.sub(r"\b%s\b" % re.escape(lit), name, new[i])
                ind = new[at][:len(new[at]) - len(new[at].lstrip())]
                new.insert(at, "%s%s = %s;" % (ind, name, lit))
                b = next(k for k, l in enumerate(new) if l.strip() == "{")
                new.insert(b + 1, "    s32 %s;" % name)
                body = "\n".join(new)
                rr, ww, _ = measure(body)
                if ww == tw and (rr, ww) < best:
                    best, found = (rr, ww), (body, "%s = %s (shift %d)" % (name, lit, shift))
                    print("  round %d: %d rows, %d words  %s" % (rnd, rr, ww, found[1]))
        if not found:
            break
        cur, tag = found[0], tag + 1
        applied.append(found[1])

    if applied and measure is not measure_unit:
        vr, vw, _ = measure_unit(cur)
        if (vr, vw) != best:
            print("constsweep: *** standalone and unit disagree (%d/%d vs %d/%d);"
                  " trusting the unit" % (vr, vw, best[0], best[1]))
            best = (vr, vw)
    print("final %d rows, %d/%d words" % (best[0], best[1], tw))
    if applied:
        out = os.path.join(sp, "best.c")
        open(out, "w").write(cur)
        print("changes: " + " | ".join(applied))
        print("saved: " + out)
    else:
        print("no constant hoist helps")


if __name__ == "__main__":
    main()
