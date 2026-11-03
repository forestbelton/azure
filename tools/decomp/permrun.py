#!/usr/bin/env python3
"""permrun.py FUNC [--minutes N] [--jobs N] -- run decomp-permuter on one park.

Parallel searches are safe with no setup. This used to need the wine prefix
pool (`make permuter-prefixes`), because concurrent ccpsx runs in one prefix
silently swapped each other's output -- measured 3 wrong in 80 at 8-way --
which a search selecting the minimum score turns into false winners. The
native compiler has no shared mutable state, so the pool is gone from the
requirements.

Prints ONE line and appends it to nonmatchings/permlog.tsv. A run costs CPU and
no context, so let it run long and read only the verdict.

    python3 tools/decomp/permimport.py func_8004D880
    python3 tools/decomp/permrun.py func_8004D880 --minutes 60 --jobs 8

A `best` below `base` means the permuter found something: its source is in
nonmatchings/FUNC/output-*/, and the gate for it is still verify.py then
make check on a cleaned-up version.

JOBS ARE A MACHINE-WIDE BUDGET OF 20, not a per-run setting. This script
counts the jobs every other running permuter already holds and clamps
`--jobs` to what is left, because the jobs are what compete for the machine
the user is also using. If the budget is already spent, it refuses rather
than queueing -- run ONE search at a time.
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
PERMUTER = os.path.join(PROJ, "vendor/decomp-permuter/permuter.py")
LOG = os.path.join(PROJ, "nonmatchings/permlog.tsv")

BASE = re.compile(r"base score = (\d+)")
ITER = re.compile(r"iteration (\d+)")
SCORE = re.compile(r"score = (\d+)")


# Machine-wide, across every concurrent permuter. The user shares this
# box with the search, so this is a ceiling on what the searches may
# take, not a target to reach. Override with AZURE_PERM_JOBS.
MAX_JOBS = int(os.environ.get("AZURE_PERM_JOBS") or 20)


def jobs_in_flight():
    """Jobs held by permuter processes other than this one's children.

    Counts only the top-level permuter.py invocations: each forks a worker
    pool whose members carry the same argv, so counting every matching
    process would multiply the figure by the pool size.
    """
    total = 0
    me = os.getpid()
    for ent in os.listdir("/proc"):
        if not ent.isdigit() or int(ent) == me:
            continue
        try:
            with open(os.path.join("/proc", ent, "cmdline"), "rb") as fh:
                argv = fh.read().split(b"\0")
        except (IOError, OSError):
            continue
        argv = [a.decode("utf-8", "replace") for a in argv if a]
        if not any(a.endswith("permuter.py") for a in argv):
            continue
        if any("forkserver" in a or "multiprocessing" in a for a in argv):
            continue
        # The `timeout` wrapper carries the same argv as the search it wraps,
        # so counting both doubles every run -- which quietly halved this
        # budget the moment the wrapper was added.
        if os.path.basename(argv[0]) == "timeout":
            continue
        if "-j" in argv:
            try:
                total += int(argv[argv.index("-j") + 1])
            except (IndexError, ValueError):
                total += 1
        else:
            total += 1
    return total


def new_session_and_die_with_parent():
    """preexec: own session (so killpg hits only the search), and PDEATHSIG.

    Without the second half, killing permrun.py orphans the search instead of
    stopping it: os.setsid() puts it out of reach of the shell's own group.
    """
    os.setsid()
    try:
        import ctypes
        PR_SET_PDEATHSIG = 1
        ctypes.CDLL("libc.so.6", use_errno=True).prctl(
            PR_SET_PDEATHSIG, signal.SIGTERM, 0, 0, 0)
    except Exception:
        pass            # a missing prctl costs promptness, not the deadline


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

    if not jobs:
        jobs = max(1, min(8, os.cpu_count() or 1))

    held = jobs_in_flight()
    free = MAX_JOBS - held
    if free < 1:
        print("permrun: REFUSING -- %d of the %d-job budget is already held "
              "by running permuters. Run ONE search at a time; wait for it "
              "or stop it first." % (held, MAX_JOBS))
        return 1
    if jobs > free:
        print("permrun: -j %d clamped to %d (%d of the %d-job budget is held "
              "by other permuters)." % (jobs, free, held, MAX_JOBS))
        jobs = free

    d = os.path.join(PROJ, "nonmatchings", func)
    if not os.path.isdir(d):
        print("permrun: no %s -- run permimport.py %s first"
              % (os.path.relpath(d, PROJ), func))
        return 1

    env = dict(os.environ)

    before = set(os.listdir(d))
    # The deadline is enforced TWICE on purpose. The loop below kills the
    # group when this process sees the clock run out, and `timeout` kills it
    # from inside the group when this process is not there to look -- which
    # happens whenever the shell that backgrounded permrun.py is reaped,
    # leaving permuter.py running in the session os.setsid() gave it with
    # nothing left to time it. That is how 16 searches accumulated on
    # 2026-10-30: each permrun.py had died, and every orphan ran past its
    # --minutes forever. PR_SET_PDEATHSIG makes an orphan die at once rather
    # than at the deadline.
    cmd = ["timeout", "--kill-after=30", "%ds" % int(minutes * 60 + 15),
           sys.executable, PERMUTER, os.path.relpath(d, PROJ),
           "-j", str(jobs), "--better-only", "--stop-on-zero"]
    proc = subprocess.Popen(
        cmd, cwd=PROJ, env=env, stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT, text=True, bufsize=1,
        preexec_fn=new_session_and_die_with_parent)

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
