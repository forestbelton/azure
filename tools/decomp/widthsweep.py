"""widthsweep.py FUNC [--rounds N] [--unit-copy PATH] -- try every DECLARED
WIDTH for every local of a parked function, greedily, and report what improves.

    python3 tools/decomp/widthsweep.py func_800A94A0

WHY THIS EXISTS. On 2026-11-21 two dungeon parks were closed most of the way
by changing ONE declaration's width, and in both cases the park's own residue
diagnosis pointed somewhere else:

    func_800B6AC4   s32 k -> s16 k    811/812 words and 46 rows -> 812/812, 39
    func_8009F644   s32 m -> u8  m    136/137 words and 81 rows -> 137/137, 42

Both were found by decomp-permuter, which reaches them only because it mutates
types among its other mutations -- half an hour of 5 jobs for a change this
tool tries in a few seconds. A park whose residue reads as a REGISTER
PERMUTATION is the strong case: a local declared wider than the target's
costs a `sll'/`sra' pair at some uses and changes a quantity's size and live
length, which moves it in `allocno_compare' and permutes everything below it.
The symptom is a shuffled callee-saved file; the cause is one declaration.

It sweeps a local at a time, keeps the best single change, and repeats, so it
finds combinations without paying for the product. A change that costs words
is rejected outright -- the word count must stay equal to the target's.

IT SWEEPS PARAMETERS AS WELL AS LOCALS, and parameters are where the biggest
win so far came from. func_800B23F8 landed on 2026-11-21 at 81/81 words from a
34-row park by declaring two parameters `u16` instead of `u8`: a parameter
declared ONE STEP WIDER than the store that consumes it gets its raw hard-register
copy deleted (combine folds it into the surviving truncation), which breaks
sched1's parameter-pinning prefix loop at the resulting NOTE_INSN_DELETED and
makes the remaining copies schedulable. u8 is 34 rows, u16 is 0, u32 is 29 --
the width is exact, not merely "narrow". See docs/matching-levers.md, "A
SUB-WORD parameter type is how a parameter copy becomes schedulable", whose
mechanism that landing corrected. The first version of this tool swept only
local declarations and reported a clean negative on that very body.

DANGER, AND IT HAS ALREADY COST A DAY: A NARROWING CAN REACH THE TARGET WORD
COUNT BY DELETING CODE THE TARGET HAS. `shorten_compare' (c-common.c:2049)
folds a comparison against a constant too wide for the narrowed type to a
CONSTANT AT TREE LEVEL, and the guard and its body vanish. On func_800ADE74
`s32 t1 -> s16' deleted the eight words of `if (t1 > 0x10000) t1 = 0xFFFF;'
-- which the target EMITS at 800ADF90..800ADFAC -- and reached 1817/1817 only
because the body was separately six words SHORT elsewhere. Two cancelling
errors read as a closed word gap, and three independent narrowings on that
function all "closed" it the same way.

The runtime value is not the test. `t1' is read from a u16 and stored back
through a u16, so the bits are identical at either width; the CODE is not, and
the code is what is being matched. SO: whenever a candidate improves the word
gap, check the aligndiff output for a CONTIGUOUS RUN OF MISSING TARGET WORDS
before believing it. A run of four or more is a deleted basic block, not a
closed gap.

A parameter width change alters the function's SIGNATURE, so a prototype
elsewhere that disagrees makes the candidate fail to compile and it scores 999
and is dropped -- the sweep cannot land a change that the unit as a whole
rejects. Read what it proposes: a narrower declared type that truncates a value
the body relies on shows up as MORE differences, and a WIDER one is only safe
when every use is narrower (which is the case this lever is about).

It will not propose a width that changes the function's behaviour: only the
declaration text is edited, so a narrower type that truncates a value the body
relies on shows up as MORE differences, not as a silent wrong match. Still
read what it suggests before landing it: `u8 m' is right because every arm
computes `m' from bytes and `q[0] = m' stores a byte, and that is a claim
about the function, not about the compiler.
"""
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
from onefunc import find_definition
import parkresidue as pr

WIDTHS = ["s32", "u32", "s16", "u16", "s8", "u8"]


def main():
    # Drop flag VALUES as well as flags, so a `--body PATH' cannot leave PATH
    # sitting in the positional list, and refuse a flag this tool does not
    # know rather than ignoring it.
    _TAKES_VALUE = ("--rounds", "--unit-copy", "--body")
    _KNOWN = _TAKES_VALUE
    args, _skip = [], False
    for _a in sys.argv[1:]:
        if _skip:
            _skip = False
            continue
        if _a.startswith("--"):
            if _a not in _KNOWN:
                raise SystemExit("widthsweep: unknown flag %s (known: %s)"
                                 % (_a, " ".join(_KNOWN)))
            _skip = _a in _TAKES_VALUE
            continue
        args.append(_a)
    if not args:
        raise SystemExit((__doc__ or "").strip().split("\n\n")[0])
    func = args[0]
    rounds = 4
    for i, a in enumerate(sys.argv):
        if a == "--rounds":
            rounds = int(sys.argv[i + 1])

    scratch = os.environ.get("AZURE_SCRATCH") or "/tmp"
    sp = os.path.join(scratch, "widthsweep", func)
    os.makedirs(sp, exist_ok=True)
    unit_path, unit = pr.unit_of(func)
    dst = os.path.join(sp, os.path.basename(unit_path))
    subprocess.run(["cp", unit_path, dst], check=True)
    r = subprocess.run([sys.executable, os.path.join(HERE, "parksplice.py"),
                        func, dst], capture_output=True, text=True)
    if "spliced" not in r.stdout:
        raise SystemExit("widthsweep: parksplice failed:\n" + r.stdout + r.stderr)

    base_txt = open(dst).read()
    st, en = find_definition(base_txt, func)
    head, fn, tail = base_txt[:st], base_txt[st:en], base_txt[en:]

    # `--body PATH' sweeps the definition in PATH instead of the park's own,
    # exactly as refsweep/devsweep/stmtsweep do.
    #
    # IT WAS MISSING HERE AND THAT IS A SILENT WRONG ANSWER, NOT A MISSING
    # FEATURE. `args' above keeps only argv entries that do NOT start with
    # `--', so a `--body /path/to/cand.c' had its flag dropped and its VALUE
    # kept as a positional -- func stayed args[0], the path was ignored, and
    # the tool swept THE PARK while reporting numbers as though it had swept
    # the candidate. The f644b agent was handed a `--body' instruction for all
    # four sweeps and three of them honoured it. A sweep that reports "clean"
    # about a body it never looked at is the most expensive kind of negative
    # this project collects.
    for _i, _a in enumerate(sys.argv):
        if _a == "--body":
            _t = open(sys.argv[_i + 1]).read()
            _s = find_definition(_t, func)
            if not _s:
                raise SystemExit("widthsweep: no definition of %s in %s"
                                 % (func, sys.argv[_i + 1]))
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

    # A SWEEP ON THE 25000-LINE UNIT IS 25 SECONDS A CANDIDATE and this tool
    # measures sixty of them per round. Build onefunc.py's standalone and
    # sweep THERE -- but only after confirming it reproduces the unit's own
    # residue exactly, which is the protocol onefunc's docstring insists on
    # and the reason the check is not optional: a standalone that declares one
    # symbol differently measures a different function.
    measure = measure_unit
    # 800, not 8000: five of the six carved dungeon units are under 8000
    # lines, so the old threshold silently disabled the standalone for all
    # of them. See refsweep.py for the full reasoning and the cost numbers.
    if len(base_txt.split("\n")) > 800:
        r1 = subprocess.run([sys.executable, os.path.join(HERE, "onefunc.py"),
                             func, "--src", dst, "--out", os.path.join(sp, "one")],
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
                print("%s: standalone measures %d rows / %d words against the unit's"
                      " %d / %d -- NOT using it" % (func, orows, ow, rows, w))
                print(r1.stdout.strip()[-400:])
    print("%s: base %d rows, %d/%d words" % (func, rows, w, tw))
    if w != tw:
        print("widthsweep: the word count already differs from the target; a width")
        print("widthsweep: change may fix that, so candidates matching %d are kept too." % tw)

    # ORDER BY WORD GAP FIRST, THEN ROWS -- not by rows alone.  A park that is
    # already word-short (func_800D13B8 at 161 of 164) or word-long
    # (func_800ADE74 at 1826 of 1817) wants the target WORD COUNT before it
    # wants a lower row count, and a strict `rows' comparison silently throws
    # away the candidate that reaches it.  measure_finds.py carries the same
    # rule in its own docstring and for the same reason.
    def key(r, ww):
        return (abs(ww - tw), r)

    cur, best = fn, (rows, w)
    best_key = key(rows, w)
    applied = []
    for rnd in range(rounds):
        # Every local declaration, as (line index, type, rest-of-line).
        lines = cur.split("\n")
        decls = []
        for i, l in enumerate(lines):
            m = re.match(r"^(\s*)(s8|u8|s16|u16|s32|u32)(\s+\**[A-Za-z_]"
                         r"[A-Za-z0-9_]*(?:\[[^\]]*\])?;)$", l)
            if m:
                decls.append((i, m.group(1), m.group(2), m.group(3)))
        if not decls:
            print("widthsweep: no plain scalar declarations found")
            return
        found = None

        # PARAMETERS, swept the same way. The signature runs from the
        # definition's first line to the `{`, and a parameter list may wrap, so
        # the text is edited in place rather than rebuilt.
        sig_end = cur.index("{")
        sig = cur[:sig_end]
        open_paren = sig.index("(")
        close_paren = sig.rindex(")")
        inner = sig[open_paren + 1:close_paren]
        parts, depth, start = [], 0, 0
        for k, ch in enumerate(inner):
            if ch in "([":
                depth += 1
            elif ch in ")]":
                depth -= 1
            elif ch == "," and depth == 0:
                parts.append((start, k)); start = k + 1
        parts.append((start, len(inner)))
        for lo, hi in parts:
            seg = inner[lo:hi]
            m = re.match(r"^(\s*)((?:const\s+)?(?:s8|u8|s16|u16|s32|u32))(\s+)"
                         r"([A-Za-z_][A-Za-z0-9_]*)\s*$", seg)
            if not m:
                continue
            for nt in WIDTHS:
                if nt == m.group(2):
                    continue
                newseg = m.group(1) + nt + m.group(3) + m.group(4)
                body = (sig[:open_paren + 1] + inner[:lo] + newseg + inner[hi:]
                        + sig[close_paren:] + cur[sig_end:])
                rr, ww, _ = measure(body)
                if key(rr, ww) < best_key:
                    best, best_key = (rr, ww), key(rr, ww)
                    found = (body, "parameter %s: %s -> %s"
                             % (m.group(4), m.group(2), nt))
                    print("  round %d: %d rows, %d words  %s"
                          % (rnd, rr, ww, found[1]))

        for i, ind, ty, rest in decls:
            for nt in WIDTHS:
                if nt == ty:
                    continue
                cand = lines[:]
                cand[i] = ind + nt + rest
                body = "\n".join(cand)
                rr, ww, _ = measure(body)
                if key(rr, ww) < best_key:
                    best, best_key = (rr, ww), key(rr, ww)
                    found = (body, "%s ->%s%s" % (ty, nt, rest))
                    print("  round %d: %d rows, %d words  %s"
                          % (rnd, rr, ww, found[1]))
        if not found:
            break
        cur = found[0]
        applied.append(found[1])

    if applied and measure is not measure_unit:
        vr, vw, _ = measure_unit(cur)
        print("verified against the real unit: %d rows, %d/%d words" % (vr, vw, tw))
        if (vr, vw) != (best[0], best[1]):
            print("widthsweep: *** THE STANDALONE AND THE UNIT DISAGREE on the")
            print("widthsweep: *** result (%d/%d vs %d/%d). Trust the unit." % (vr, vw, best[0], best[1]))
            best, best_key = (vr, vw), key(vr, vw)
    print("final %d rows, %d/%d words" % (best[0], best[1], tw))
    if applied and best[1] < w:
        print("widthsweep: *** THIS RESULT IS SHORTER THAN THE BODY YOU STARTED")
        print("widthsweep: *** WITH (%d -> %d words). A narrowing can reach the" % (w, best[1]))
        print("widthsweep: *** target count by DELETING code the target has --")
        print("widthsweep: *** shorten_compare folds a comparison against a too-wide")
        print("widthsweep: *** constant to a constant and the guard disappears.")
        print("widthsweep: *** Check aligndiff for a contiguous run of MISSING")
        print("widthsweep: *** target words before applying this. See the docstring.")
    if applied:
        out = os.path.join(sp, "best.c")
        open(out, "w").write(cur)
        print("changes: " + " | ".join(applied))
        print("saved: " + out)
    else:
        print("no width change helps")


if __name__ == "__main__":
    main()
