#!/usr/bin/env python3
"""perm.py BASE.c UNIT FUNC -- permute the statements between /* PERM */ markers.

Statement ORDER is the lever cc1psx is most sensitive to and the one that is
pure search: there is nothing to derive, only N! arrangements to score. Mark
the block to permute in the candidate and this tries every ordering, scoring
each with verify.py exactly as search.py does:

    void func_80047738(u8 *p, s16 a, s16 b) {
        u8 *q = ...;
        /* PERM */
        *(u8 **) p = q;
        *(u16 *) (p + 0x14) &= 0xFFBF;
        p[4] = b;
        /* ENDPERM */
    }

One marked line is one unit. A unit may span several lines by ending each
continued line with a backslash, or by indenting the continuation more deeply
than the first line -- a brace block works that way too. Blank lines and
comments inside the block travel with the unit above them.

Score is (|built words - target words|, differing words), lexicographic, the
same as search.py. 7 units is 5040 permutations at about 0.7s each, so pass
--max to cap the run; it samples randomly above the cap.
"""
import itertools, os, random, re, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.environ.get("AZURE_ROOT", os.path.dirname(os.path.dirname(HERE)))
SP = os.environ.get("AZURE_SCRATCH", "/tmp/azure-decomp")
os.makedirs(SP, exist_ok=True)

argv, cap = [], None
it = iter(sys.argv[1:])
for a in it:
    if a == "--max":
        cap = int(next(it))
    else:
        argv.append(a)
src, unit, func = argv[0:3]
text = open(src).read()
lines = text.split("\n")
try:
    i = next(n for n, l in enumerate(lines) if "/* PERM */" in l)
    j = next(n for n, l in enumerate(lines) if "/* ENDPERM */" in l)
except StopIteration:
    sys.exit("no /* PERM */ .. /* ENDPERM */ block in " + src)
head, body, tail = lines[:i], lines[i + 1:j], lines[j + 1:]

# group the body into units: a line whose indent is deeper than the previous
# unit's first line, or which follows a backslash, continues that unit.
units, base_indent, in_comment = [], None, False
for l in body:
    ind = len(l) - len(l.lstrip())
    cont = (units and (l.strip() == "" or in_comment
                       or units[-1][-1].rstrip().endswith("\\")
                       or (l.strip() and base_indent is not None and ind > base_indent)))
    if "/*" in l and "*/" not in l.split("/*", 1)[1]:
        in_comment = True
    elif "*/" in l:
        in_comment = False
    if cont:
        units[-1].append(l)
    else:
        units.append([l])
        if l.strip():
            base_indent = ind
units = [u for u in units if any(x.strip() for x in u)]
n = len(units)
print("%d unit(s), %d permutation(s)" % (n, __import__("math").factorial(n)))
for k, u in enumerate(units):
    print("  %d: %s" % (k + 1, u[0].strip()[:70]))

perms = list(itertools.permutations(range(n)))
if cap and len(perms) > cap:
    first = perms[0]
    perms = random.sample(perms, cap - 1)
    perms.insert(0, first)

scratch = os.path.join(SP, "perm_%s.c" % func)
best, seen = (10 ** 9, 10 ** 9, None), {}
for p in perms:
    out = head + sum((units[k] for k in p), []) + tail
    open(scratch, "w").write("\n".join(out))
    r = subprocess.run([sys.executable, os.path.join(HERE, "verify.py"), scratch, unit, func],
                       capture_output=True, text=True, cwd=PROJ)
    m = re.search(r"built (\d+) words, target (\d+) words", r.stdout)
    res = re.search(r"RESULT: (MATCH|(\d+) DIFF)", r.stdout)
    if not m or not res:
        print(" ".join(str(k + 1) for k in p), "ERROR", r.stdout[-200:] or r.stderr[-200:])
        continue
    dw = abs(int(m.group(1)) - int(m.group(2)))
    nd = 0 if res.group(1) == "MATCH" else int(res.group(2))
    score = (dw, nd, p)
    if score[:2] < best[:2]:
        best = score
        print(" ".join(str(k + 1) for k in p), "words%+d" % (int(m.group(1)) - int(m.group(2))),
              res.group(1), "  <-- best", flush=True)
        if nd == 0 and dw == 0:
            break
print("\nbest: order %s, %d word(s) off, %d DIFF" %
      (" ".join(str(k + 1) for k in best[2]), best[0], best[1]))
