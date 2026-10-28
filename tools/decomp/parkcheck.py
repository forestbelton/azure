#!/usr/bin/env python3
"""parkcheck.py [FUNC ...] -- every park's HEADLINE must match a fresh build.

`docs/near-misses/<func>.c`'s first line has to read "N of M words, D DIFF",
and the next session reads that line instead of compiling. So a stale headline
is not untidiness, it is a wrong input: a park recorded at 22 DIFF that really
builds at 18 gets skipped as "further away" than one at 20, and the whole
cheapest-first order is decided on those numbers.

Four things it catches, each of which has actually happened here:

  * A HEADLINE THAT NO LONGER MEASURES. Adopting an improvement and forgetting
    the first line is the common case -- three rows were stale on 2026-10-27.
  * A PARK THAT DOES NOT BUILD AT ALL. func_8003F368 carried a stray `*/` for
    days; `search.py` printed "no candidates" and `advise.py` an empty verdict,
    because both compile the file and neither says so when the compile fails.
  * A UNIT THAT IS NOT THE LISTING'S. The headline's `(unit)` is what every
    tool passes to verify.py, and the asm for a carved function can exist under
    more than one unit directory.
  * A SIGNATURE THAT DISAGREES WITH THE PROTOTYPE IN `csrc/` -- reported as a
    warning, because it is a hazard at LANDING time rather than an error now.
    The prototype in scope is what every caller compiled against, so landing a
    park whose signature differs rewrites those callers; func_8003AB44's
    landing needed exactly this noticing, where a missing declaration had been
    passing a `u8` argument untruncated. The prototypes for parked functions
    are placeholders and are NOT evidence about parameter widths -- checked
    2026-10-28 across all twelve current mismatches, and in every one the
    caller's own argument is already the narrower type, so the declared width
    changes no caller code either way. Treat the list as a landing checklist,
    not as a finding.

With no arguments it checks every park. Name functions to check only those.
Give it its own wine prefix if anything else is compiling (see unname.py).
"""
import glob, io, os, re, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.environ.get("AZURE_ROOT", os.path.dirname(os.path.dirname(HERE)))

want = set(sys.argv[1:])
bad = []
checked = 0
for f in sorted(glob.glob(os.path.join(PROJ, "docs/near-misses/*.c"))):
    func = os.path.basename(f)[:-2]
    if want and func not in want:
        continue
    head = io.open(f, encoding="utf-8").read().split("\n")[0]
    m = re.search(r"\((\w+)\)", head)
    if not m:
        continue                       # probe files, not parks
    unit = m.group(1)
    checked += 1
    if not glob.glob(os.path.join(PROJ, "asm/nonmatchings/%s/%s.s" % (unit, func))):
        bad.append((func, "headline says unit %s, but no listing there" % unit))
        continue
    out = subprocess.run([sys.executable, os.path.join(HERE, "verify.py"), f, unit, func],
                         capture_output=True, text=True, cwd=PROJ).stdout
    w = re.search(r"built (\d+) words, target (\d+) words", out)
    r = re.search(r"RESULT: (MATCH|(\d+) DIFF)", out)
    if not w or not r:
        bad.append((func, "does not build or verify"))
        continue
    real = (int(w.group(1)), int(w.group(2)), 0 if r.group(1) == "MATCH" else int(r.group(2)))
    claim = re.search(r"(\d+) of (\d+) words, (\d+) DIFF", head)
    if not claim:
        bad.append((func, 'first line has no "N of M words, D DIFF"'))
        continue
    c = tuple(int(x) for x in claim.groups())
    if c != real:
        bad.append((func, "claims %d of %d words / %d DIFF, builds %d of %d / %d"
                    % (c + real)))

# Signature against the prototype every caller compiled against.
import itertools
csrc = "\n".join(io.open(q, encoding="utf-8", errors="replace").read()
                 for q in glob.glob(os.path.join(PROJ, "csrc/*.c")))
warn = []
for f in sorted(glob.glob(os.path.join(PROJ, "docs/near-misses/*.c"))):
    func = os.path.basename(f)[:-2]
    if want and func not in want:
        continue
    text = io.open(f, encoding="utf-8").read()
    d = re.search(r"^[A-Za-z_][\w \*]*\b%s\s*\(([^)]*)\)\s*$" % re.escape(func),
                  text, re.M)
    if not d:
        continue
    mine = [re.sub(r"\s+\w+$", "", x).strip().replace(" ", "")
            for x in d.group(1).split(",")] if d.group(1).strip() not in ("", "void") else []
    for proto in set(re.findall(r"^[A-Za-z_][\w \*]*\b%s\s*\(([^)]*)\)\s*;" % re.escape(func),
                                csrc, re.M)):
        theirs = [x.strip().replace(" ", "") for x in proto.split(",")] \
            if proto.strip() not in ("", "void") else []
        if mine != theirs:
            warn.append((func, d.group(1).strip(), proto.strip()))

print("%d park(s) checked, %d problem(s)" % (checked, len(bad)))
for func, why in bad:
    print("  %-16s %s" % (func, why))
if warn:
    print("\n%d signature(s) to correct when the park lands (not an error):" % len(warn))
    for func, mine, theirs in sorted(set(warn)):
        print("  %-16s park(%s)" % (func, mine))
        print("  %-16s csrc(%s)" % ("", theirs))
sys.exit(1 if bad else 0)
