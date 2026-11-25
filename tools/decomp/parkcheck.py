#!/usr/bin/env python3
"""parkcheck.py [FILE...] -- structural check on the park directories.

Both of them: `docs/dungeon-parks/` for the resident overlays and
`docs/dng-parks/` for DUNGEON.BIN's 116 carved objects. The second was added
with the carve and went unscanned until a park landed in it, which is the
quiet way a checker stops checking.

These files are a function BODY under a hand-written header comment. They are
not compiled by anything, so nothing catches a malformed one -- and on
2026-11-16 three of thirty-five were malformed at once, two of them by my own
edits. The failure is always the same: an inner `/*` inside the header, which
in C terminates the OUTER comment at the first following `*/` and turns the
rest of the header into code.

It survives unnoticed because every tool that reads a park extracts from the
DEFINITION LINE onward and never sees the header, so the body measures fine
while the file as a whole is not valid C.

    python3 tools/decomp/parkcheck.py            # every park, exit 1 if any is bad
    python3 tools/decomp/parkcheck.py FILE ...   # just these
    python3 tools/decomp/parkcheck.py --splice   # ALSO check each one still
                                                 # SPLICES AND COMPILES

## STRUCTURAL IS NOT MEASURABLE, AND `0 bad' NEVER MEANT WHAT I READ IT AS

The check above is structural only. A park can pass it and still be
impossible to measure, because **a park decays as its own unit lands
beneath it.** Its DECLARATIONS block was written against the agent's copy;
every body the dispatcher then integrates changes the unit's real block,
and sooner or later the two disagree and nothing splices.

That is not hypothetical and it is not slow: a park committed on
2026-11-24 was unmeasurable within the hour, because the five bodies
landed alongside it went into `csrc/` and the park's block no longer
agrees with the unit's. `parkcheck` printed "9 parks, 0 bad" for it.

Parks are the backlog of the hardest remaining work and 10 of 13 were
resurrected in one sweep -- but only because they could still be
measured. Silent rot takes that away, so `--splice` exists to find it.
It is one compile per park, which is slower than the structural pass and
far cheaper than discovering it when an agent picks the park up.
"""
import glob
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.environ.get("AZURE_ROOT", os.path.dirname(os.path.dirname(HERE)))


def check(path):
    """Return a complaint string, or None."""
    try:
        s = open(path, encoding="utf-8").read()
    except OSError as e:
        return str(e)
    depth = 0
    for m in re.finditer(r"/\*|\*/", s):
        if m.group() == "/*":
            if depth:
                line = s.count("\n", 0, m.start()) + 1
                return ("inner `/*` at line %d -- it ends the header comment "
                        "early and orphans the rest" % line)
            depth = 1
        else:
            if not depth:
                line = s.count("\n", 0, m.start()) + 1
                return "stray `*/` at line %d" % line
            depth = 0
    if depth:
        return "unterminated comment"

    name = os.path.basename(path)[:-2]

    # The header must be ONE comment. On 2026-11-16 my own repair of the
    # inner-"/*" bug rewrote each inner opener to "(*" but left its matching
    # "*/", which closed the OUTER comment early and dropped the rest of the
    # header to top level as bare prose -- and this check, which then only
    # looked for a nested "/*", passed all three files. clang-format had
    # already mangled one of them before anyone noticed.
    m = None
    for mm in re.finditer(r"^[A-Za-z_][\w \*]*\b%s\s*\(" % re.escape(name), s, re.M):
        m = mm
    if m:
        head = s[:m.start()].strip()
        if head.startswith("/*"):
            # Strip EVERY well-formed comment, not just the first. A park may
            # legitimately follow its header with a second commented block of
            # declarations and typedefs, and checking only after the first
            # "*/" reported that block's prose as orphaned.
            rest = re.sub(r"/\*.*?\*/", "", head, flags=re.S)
            for n, ln in enumerate(rest.split("\n")):
                t = ln.strip()
                if not t or t.startswith(("#", "/*", "*", "}", "{", "//")):
                    continue
                if t.endswith((";", "{", "}", ")", ":", "*/", ",")):
                    continue
                if len(t.split()) >= 4:          # a sentence, not C
                    return ("prose OUTSIDE the header comment: %r -- the "
                            "header closed early" % t[:50])
                break
    if not re.search(r"^[A-Za-z_][\w \*]*\b%s\s*\(" % re.escape(name), s, re.M):
        return ("no line defining %s at column 0 -- the merge scripts find the "
                "body by that line" % name)
    return None


def splices(path):
    """None if the park still splices and compiles, else the reason."""
    import subprocess
    import tempfile
    func = os.path.basename(path)[:-2]
    with tempfile.TemporaryDirectory() as d:
        # parksplice insists on the unit's own basename, and derives it itself.
        r = subprocess.run(
            [sys.executable, os.path.join(HERE, "parksplice.py"), func,
             os.path.join(d, "PLACEHOLDER.c")],
            capture_output=True, text=True)
        out = r.stdout + r.stderr
        m = re.search(r"must be ([A-Za-z0-9_.]+\.c)", out)
        if not m:
            return None if r.returncode == 0 else (out.strip().split("\n")[0] or
                                                   "parksplice failed")
        r = subprocess.run(
            [sys.executable, os.path.join(HERE, "parksplice.py"), func,
             os.path.join(d, m.group(1))],
            capture_output=True, text=True)
        if r.returncode == 0:
            return None
        out = (r.stdout + r.stderr).strip().split("\n")
        return next((l.strip() for l in out if "rror" in l or "NO placement" in l),
                    out[0] if out else "parksplice failed")


def main():
    argv = [a for a in sys.argv[1:] if a != "--splice"]
    do_splice = "--splice" in sys.argv[1:]
    files = argv or sorted(
        glob.glob(os.path.join(PROJ, "docs/dungeon-parks/func_*.c"))
        + glob.glob(os.path.join(PROJ, "docs/dng-parks/func_*.c")))
    bad = 0
    # A PARK FOR A FUNCTION THAT IS NO LONGER A STUB IS STALE, and this check
    # reported "0 bad" while exactly that existed: the permuter closed a
    # 520-word park, I landed the body and staged the park file in the same
    # commit without deleting it. Nothing in the structural check looks at
    # whether the function is still parked, so the park count drifts upward
    # and a later agent can be handed a park for a body that is already done.
    try:
        sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
        import dngfam
        stubs = dngfam.stubs()
    except Exception:
        stubs = None
    for p in files:
        c = check(p)

        if c:
            print("%s: %s" % (os.path.relpath(p, PROJ), c))
            bad += 1
            continue
        if stubs is not None:
            fn = os.path.basename(p)[:-2]
            if fn not in stubs:
                print("%s: STALE -- %s is LANDED; delete this park"
                      % (os.path.relpath(p, PROJ), fn))
                bad += 1
                continue
        if do_splice:
            why = splices(p)
            if why:
                print("%s: STRUCTURALLY OK BUT WILL NOT SPLICE -- %s"
                      % (os.path.relpath(p, PROJ), why))
                bad += 1
    print("%d park%s checked%s, %d bad"
          % (len(files), "" if len(files) == 1 else "s",
             " (incl. splice)" if do_splice else "", bad))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
