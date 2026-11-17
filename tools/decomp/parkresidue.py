#!/usr/bin/env python3
"""parkresidue.py [--jobs N] [FUNC...] -- MEASURE every park, don't trust its header.

A park header states the residue its author measured. That number goes stale:
the unit changes underneath it as declarations are added and neighbours land,
and a header can simply be wrong. On 2026-11-16 I ranked a batch by scraping
headers with a regex and told an agent a park was at 6 differing when it was
at 33 -- the regex had matched a number from the prose.

This splices each park's body into the CURRENT csrc/ovl_dungeon.c and runs
aligndiff, so the ranking is a measurement. It also catches a park that no
longer compiles, which is the more valuable failure to find early.

    python3 tools/decomp/parkresidue.py                  # all parks, ranked
    python3 tools/decomp/parkresidue.py func_800A2424    # just one
"""
import concurrent.futures as cf
import glob
import os
import re
import subprocess
import sys
import tempfile

PROJ = os.environ.get("AZURE_ROOT", "/home/case/projects/azure")
UNIT = os.path.join(PROJ, "csrc/ovl_dungeon.c")


DECL_RE = re.compile(
    r"^\s*DECLARATIONS\s*\n(.*?)^\s*END DECLARATIONS\s*$",
    re.M | re.S)


def decls_of(path):
    """Extract a park's DECLARATIONS block, if it has one.

    A park body is NOT self-contained: it needs externs, typedefs and callee
    prototypes that live in the unit's top block. Those were historically
    reported only in the agent's hand-back message -- which is a transcript,
    not the repo -- so 19 of 34 parks could not be compiled from the repo
    alone on 2026-11-16. A park states them in its header like this, inside
    the comment:

        DECLARATIONS
        extern u8 D_800E0979[];
        void func_80067E2C(void *, void *);
        END DECLARATIONS
    """
    s = open(path, encoding="utf-8").read()
    m = DECL_RE.search(s)
    if not m:
        return ""
    # strip comment-continuation whitespace, keep the lines themselves
    return "".join(l.strip() + "\n" for l in m.group(1).split("\n") if l.strip())


def body_of(path, func):
    s = open(path, encoding="utf-8").read()
    m = None
    for mm in re.finditer(r"^[A-Za-z_][\w \*]*\b%s\s*\([^;]*\)\s*$" % re.escape(func),
                          s, re.M):
        m = mm
    if not m:
        return None
    return s[m.start():]


def measure(func):
    park = os.path.join(PROJ, "docs/dungeon-parks/%s.c" % func)
    body = body_of(park, func)
    if body is None:
        return func, None, "no definition line in park"
    unit = open(UNIT, encoding="utf-8").read()
    stub = 'INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", %s);' % func
    if unit.count(stub) != 1:
        return func, None, "not a stub in the unit (landed already?)"
    d = tempfile.mkdtemp(prefix="parkres-")
    src = os.path.join(d, "ovl_dungeon.c")
    text = unit.replace(stub, body.rstrip() + "\n")
    decls = decls_of(park)
    if decls:
        anchor = "extern u8 D_80082E80[];\n"
        if anchor in text:
            text = text.replace(anchor, anchor + decls, 1)
    open(src, "w").write(text)
    env = dict(os.environ, AZURE_SCRATCH=os.path.join(d, "sp"))
    os.makedirs(env["AZURE_SCRATCH"], exist_ok=True)
    r = subprocess.run([sys.executable, "tools/decomp/aligndiff.py", src, func],
                       capture_output=True, text=True, env=env, cwd=PROJ)
    m = re.search(r"built (\d+) words, target (\d+) words, (\d+) aligned", r.stdout)
    if not m:
        err = (r.stderr or r.stdout).strip().split("\n")
        err = [l for l in err if "error" in l.lower() or "undeclared" in l.lower()]
        return func, None, (err[-1][:90] if err else "did not compile")
    built, tgt, nd = int(m.group(1)), int(m.group(2)), int(m.group(3))
    return func, (nd, built, tgt), None


def main():
    argv = sys.argv[1:]
    jobs, args, skip = 4, [], False
    for i, a in enumerate(argv):
        if skip:
            skip = False
            continue
        if a == "--jobs":
            jobs = int(argv[i + 1])
            skip = True            # the VALUE is not a function name
        elif not a.startswith("-"):
            args.append(a)
    funcs = args or sorted(
        os.path.basename(p)[:-2]
        for p in glob.glob(os.path.join(PROJ, "docs/dungeon-parks/func_*.c")))
    rows, bad = [], []
    with cf.ThreadPoolExecutor(max_workers=jobs) as ex:
        for func, res, err in ex.map(measure, funcs):
            (bad if err else rows).append((func, err) if err else (res, func))
    rows.sort()
    print("%-18s %6s  %s" % ("function", "rows", "words"))
    for (nd, built, tgt), func in rows:
        flag = "" if built == tgt else "   <- %+d words" % (built - tgt)
        print("%-18s %6d  %d/%d%s" % (func, nd, built, tgt, flag))
    for func, err in bad:
        print("%-18s   SKIP  %s" % (func, err))
    print("\n%d measured, %d skipped" % (len(rows), len(bad)))


if __name__ == "__main__":
    main()
