#!/usr/bin/env python3
"""Rank a permuter run's candidates by ALIGNED DIFFERENCES, not by its score.

THE PERMUTER'S SCORE IS NOT THE METRIC WE CARE ABOUT, and on a large body it
is only weakly related to it. Measured on `func_b12_80173FDC`, 745 candidates
from a 50-minute run:

  * its own best score (2285) measures 90 aligned differences in one variant
    and 106 in another -- SAME SCORE, 16 apart, so the score does not even
    determine the answer;
  * the best aligned result in an 11-point sample was 81, at the MIDDLING
    score of 3060;
  * the correlation between score and aligned differences was +0.31;
  * and the park's own clean body was 97, so the run did contain a real
    improvement -- just not where `permrun.py` pointed.

`permrun.py` prints `IMPROVED: 3230 -> 2285` and names that directory. Acting
on that line alone picks a body WORSE than the one you started with. This
script compiles every candidate into the real unit and sorts by the number
that matters.

Usage:
    python3 tools/decomp/permrank.py FUNC [-j N] [--top N] [--park PATH]

A park's DECLARATIONS block is spliced in when the unit does not already
declare what the body needs (an invented typedef, usually), which is the same
trap that made a 50-minute run report `base=?' and do nothing.

!! THE RANKED LIST IS LEADS TO READ, NOT BODIES TO LAND. !!

decomp-permuter DOES NOT PRESERVE SEMANTICS. It mutates source until the
objective improves, and a mutation that happens to compile is kept whether or
not it still computes the right thing. The top candidate in the run above --
70 aligned differences, the best of 745 -- contained

    xr = (zr -= 8);

in place of the two statements `zr -= 8; xr -= 8;'. `zr' is an `s16 *' into
the depth array at 0x1F800100 and `xr' an `s32 *' into the vertex array at
0x1F800000, so that line aims one pointer at the other array. It is WRONG
CODE that merely assembles close to the target.

So the workflow is: rank by this column, then READ the top candidates and
extract only the changes that are semantics-preserving, re-measuring each on
its own. On that run the single legitimate idea -- routing a `0x40' through
the loop variable to buy it one allocno reference -- was worth 20 of the 27
differences the permuter claimed, and the other 7 came from the broken
pointer assignment.
"""
import argparse
import concurrent.futures
import glob
import os
import re
import shutil
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dngclone  # noqa: E402
import parkresidue  # noqa: E402

PROJ = dngclone.PROJ
DEF_RE = re.compile(
    r"\n(?=(?:/\*(?:[^*]|\*(?!/))*\*/\s*)?\w[\w \t\*]*\n?\w*\s*\([^;]*\)\s*\n\{)")


def prepared_unit(func, park):
    """The unit's text with the park's declarations spliced in, if any."""
    text = open(dngclone.unit_path(func)).read()
    if not park or not os.path.exists(park):
        return text
    decls = parkresidue.decls_of(park)
    if not decls.strip():
        return text
    m = DEF_RE.search(text)
    at = m.start() if m else len(text)
    return text[:at] + "\n" + decls + "\n" + text[at:]


def score_of(path):
    m = re.search(r"output-(\d+)-", path)
    return int(m.group(1)) if m else -1


def measure(args):
    func, pre, src, workdir = args
    body, _ = dngclone.definition(open(src).read(), func)
    if body is None:
        return (src, None, None, "no definition")
    os.makedirs(workdir, exist_ok=True)
    unit = os.path.join(workdir, os.path.basename(dngclone.unit_path(func)))
    open(unit, "w").write(dngclone.splice(pre, func, body))
    env = dict(os.environ, AZURE_SCRATCH=os.path.join(workdir, "sp"))
    def run(tool):
        return subprocess.run(
            [sys.executable, os.path.join(PROJ, "tools", "decomp", tool),
             unit, func], capture_output=True, text=True, env=env, cwd=PROJ)
    r = run("ovlverify.py")
    w = re.search(r"candidate (\d+) words", r.stdout)
    if not w:
        return (src, None, None,
                (r.stdout + r.stderr).strip().splitlines()[-1][:70]
                if (r.stdout or r.stderr) else "no verdict")
    a = run("aligndiff.py")
    al = re.search(r"(\d+)\s+aligned", a.stdout)
    return (src, int(w.group(1)), int(al.group(1)) if al else None, "")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("func")
    ap.add_argument("-j", type=int, default=8)
    ap.add_argument("--top", type=int, default=15)
    ap.add_argument("--park", default=None,
                    help="park file whose DECLARATIONS the body needs "
                         "(default: docs/dng-parks/<func>.c if present)")
    a = ap.parse_args()

    park = a.park or os.path.join(PROJ, "docs", "dng-parks", a.func + ".c")
    pre = prepared_unit(a.func, park)
    outs = sorted(glob.glob(os.path.join(
        PROJ, "nonmatchings", a.func, "output-*", "source.c")), key=score_of)
    if not outs:
        raise SystemExit("no candidates under nonmatchings/%s/output-*/" % a.func)
    print("%d candidate(s), permuter scores %d..%d"
          % (len(outs), score_of(outs[0]), score_of(outs[-1])))

    td = tempfile.mkdtemp(prefix="permrank-")
    try:
        jobs = [(a.func, pre, s, os.path.join(td, "w%d" % (i % a.j)))
                for i, s in enumerate(outs)]
        rows, bad = [], 0
        with concurrent.futures.ThreadPoolExecutor(max_workers=a.j) as ex:
            for n, (src, w, al, err) in enumerate(ex.map(measure, jobs), 1):
                if al is None:
                    bad += 1
                else:
                    rows.append((al, w, score_of(src), src))
                if n % 50 == 0:
                    print("  measured %d/%d" % (n, len(outs)), file=sys.stderr)
    finally:
        shutil.rmtree(td, ignore_errors=True)

    rows.sort()
    print("\n%-9s %-7s %-9s %s" % ("aligned", "words", "its score", "source"))
    for al, w, sc, src in rows[:a.top]:
        print("%-9d %-7d %-9d %s" % (al, w, sc, os.path.relpath(
            os.path.dirname(src), PROJ)))
    if bad:
        print("\n%d candidate(s) did not compile or measure" % bad)
    if rows:
        best = rows[0]
        byscore = min(rows, key=lambda r: r[2])
        print("\nBEST BY ALIGNED DIFFERENCES: %d (%d words) at score %d"
              % (best[0], best[1], best[2]))
        print("THE PERMUTER'S OWN BEST SCORE: %d, which measures %d aligned"
              % (byscore[2], byscore[0]))
        if best[0] != byscore[0]:
            print("They disagree -- rank by this column, not by the score.")


if __name__ == "__main__":
    main()
