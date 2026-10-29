#!/usr/bin/env python3
"""permrun.py FUNC [--minutes N] [--jobs N] -- run decomp-permuter on one park.

Needs the wine prefix pool (`make permuter-prefixes`): concurrent ccpsx runs
in one prefix silently swap each other's output, which a search selecting the
minimum score turns into false winners. tools/ccpsx claims a pooled prefix per
compile when the pool exists, measured 0 wrong in 80 at 8-way against 3 wrong
in 80 shared, at the same speed.

Prints ONE line and appends it to nonmatchings/permlog.tsv. A run costs CPU and
no context, so let it run long and read only the verdict.

    python3 tools/decomp/permimport.py func_8004D880
    python3 tools/decomp/permrun.py func_8004D880 --minutes 60 --jobs 8

A `best` below `base` means the permuter found something: its source is in
nonmatchings/FUNC/output-*/, and the gate for it is still verify.py then
make check on a cleaned-up version.
"""
import datetime
import io
import os
import re
import signal
import subprocess
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.environ.get("AZURE_ROOT", os.path.dirname(os.path.dirname(HERE)))
CACHE = os.environ.get("XDG_CACHE_HOME", os.path.expanduser("~/.cache"))
POOL = os.path.join(CACHE, "azure-psyq-pool")
PERMUTER = os.path.join(PROJ, "vendor/decomp-permuter/permuter.py")
LOG = os.path.join(PROJ, "nonmatchings/permlog.tsv")

BASE = re.compile(r"base score = (\d+)")
ITER = re.compile(r"iteration (\d+)")
SCORE = re.compile(r"score = (\d+)")


def pool_size():
    if not os.path.isdir(POOL):
        return 0
    return len([n for n in os.listdir(POOL)
                if n.startswith("p") and os.path.isdir(os.path.join(POOL, n))])


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    if not args:
        print((__doc__ or "").strip().split("\n\n")[0])
        return 2
    func = args[0]
    minutes = 60.0
    jobs = 0
    for a in sys.argv[1:]:
        if a.startswith("--minutes="):
            minutes = float(a.split("=", 1)[1])
        elif a.startswith("--jobs="):
            jobs = int(a.split("=", 1)[1])
    if "--minutes" in sys.argv:
        minutes = float(sys.argv[sys.argv.index("--minutes") + 1])
    if "--jobs" in sys.argv:
        jobs = int(sys.argv[sys.argv.index("--jobs") + 1])

    npool = pool_size()
    if not jobs:
        jobs = max(1, min(8, npool))
    if jobs > 1 and npool < jobs:
        print("permrun: REFUSING -j %d with %d pooled prefixes. Concurrent "
              "compiles in one prefix swap outputs and manufacture false "
              "winners; run `make permuter-prefixes POOL=%d` or pass --jobs 1."
              % (jobs, npool, jobs))
        return 1

    d = os.path.join(PROJ, "nonmatchings", func)
    if not os.path.isdir(d):
        print("permrun: no %s -- run permimport.py %s first"
              % (os.path.relpath(d, PROJ), func))
        return 1

    env = dict(os.environ)
    env.pop("AZURE_WINEPREFIX", None)   # let tools/ccpsx claim per compile

    before = set(os.listdir(d))
    proc = subprocess.Popen(
        [sys.executable, PERMUTER, os.path.relpath(d, PROJ), "-j", str(jobs),
         "--better-only", "--stop-on-zero"],
        cwd=PROJ, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
        text=True, bufsize=1, preexec_fn=os.setsid)

    base = best = None
    iters = 0
    t0 = time.time()
    deadline = t0 + minutes * 60
    try:
        assert proc.stdout is not None
        while True:
            if time.time() > deadline:
                os.killpg(os.getpgid(proc.pid), signal.SIGTERM)
                break
            line = proc.stdout.readline()
            if not line:
                break
            m = BASE.search(line)
            if m:
                base = int(m.group(1))
                best = base if best is None else best
            m = ITER.search(line)
            if m:
                iters = max(iters, int(m.group(1)))
            for m in SCORE.finditer(line):
                v = int(m.group(1))
                if best is None or v < best:
                    best = v
    except KeyboardInterrupt:
        os.killpg(os.getpgid(proc.pid), signal.SIGTERM)
    finally:
        try:
            proc.wait(timeout=20)
        except subprocess.TimeoutExpired:
            os.killpg(os.getpgid(proc.pid), signal.SIGKILL)

    elapsed = time.time() - t0
    found = sorted(n for n in set(os.listdir(d)) - before if n.startswith("output-"))
    row = "\t".join([
        datetime.date.today().isoformat(), func,
        "%.0fm" % (elapsed / 60.0), "%d" % iters,
        "base=%s" % ("?" if base is None else base),
        "best=%s" % ("?" if best is None else best),
        "rate=%.1f/s" % (iters / elapsed if elapsed else 0),
        ("found=" + ",".join(found)) if found else "found=none",
        "jobs=%d" % jobs,
    ])
    with io.open(LOG, "a", encoding="utf-8") as fh:
        fh.write(row + "\n")
    print(row)
    if base is not None and best is not None and best < base:
        print("IMPROVED: %d -> %d. Source in %s/output-*/; gate is verify.py "
              "then make check." % (base, best, os.path.relpath(d, PROJ)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
