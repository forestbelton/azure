"""measure_finds.py FUNC [N] -- measure a permuter run's best N finds against
the REAL unit, because the permuter's score is not the row count.

    python3 tools/decomp/measure_finds.py func_800B490C 6

It splices FUNC's park into its own unit (via parkresidue.unit_of, so the
carved units resolve), measures that as the baseline, then substitutes each
of the N best-scoring `nonmatchings/FUNC/output-*/source.c' bodies and
measures each one.  A find that beats the park is written to
$AZURE_SCRATCH/../best.c with its row count printed.

WHY THIS IS NOT OPTIONAL: THE TWO METRICS DISAGREE, AND THEY DISAGREE MOST
WHERE IT MATTERS.  decomp-permuter scores the whole object with its own diff;
aligndiff counts rows in the LCS alignment of this function's words.  On
2026-11-20:

  * func_800B490C -- park at 18 rows.  The permuter's best find measured 7.
    Worth 11 rows and found in 30 minutes.
  * func_8009A3D0 -- park at 2 rows.  SIX finds that scored better than the
    park measured 10, 10, 12, 13, 13 and 14 rows.  Every one was worse.

So the permuter earns its keep on a park with real headroom and is actively
misleading once a park is near the floor -- below roughly ten rows its metric
and the row count point in different directions.  Never apply a find because
permrun logged a better score; measure it here first.  And never read
`best=NNN' in nonmatchings/permlog.tsv as a row count.
"""
import os, re, sys, glob, subprocess
sys.path.insert(0, 'tools/decomp')
from onefunc import find_definition
import parkresidue as pr

func = sys.argv[1]
topn = int(sys.argv[2]) if len(sys.argv) > 2 else 8
SP = os.path.join(os.environ['SPD'], 'mf', func)
os.makedirs(os.path.join(SP, 'sp'), exist_ok=True)
os.environ['AZURE_SCRATCH'] = os.path.join(SP, 'sp')
os.environ['TMPDIR'] = SP
unit_path, unit = pr.unit_of(func)
dst = os.path.join(SP, os.path.basename(unit_path))
subprocess.run(['cp', unit_path, dst], check=True)
r = subprocess.run(['python3', 'tools/decomp/parksplice.py', func, dst],
                   capture_output=True, text=True)
print(r.stdout.strip().split('\n')[-1][-90:])
base = open(dst).read()
bs, be = find_definition(base, func)
m = re.search(r'(\d+) aligned differences', r.stdout)
cur = int(m.group(1)) if m else 999
outs = sorted(glob.glob('nonmatchings/%s/output-*/source.c' % func),
              key=lambda p: int(p.split('output-')[1].split('-')[0]))[:topn]
best = (cur, None, '')
for o in outs:
    cand = open(o).read()
    sp = find_definition(cand, func)
    if not sp:
        print('  %-18s no definition found' % o.split('/')[-2]); continue
    open(dst, 'w').write(base[:bs] + cand[sp[0]:sp[1]] + base[be:])
    rr = subprocess.run(['python3', 'tools/decomp/aligndiff.py', dst, func],
                        capture_output=True, text=True)
    line = (rr.stdout.strip() or 'ERR').split('\n')[0]
    mm = re.match(r'built (\d+) words, target (\d+) words, (\d+)', line)
    tag = ''
    if mm and mm.group(1) == mm.group(2) and int(mm.group(3)) < best[0]:
        best = (int(mm.group(3)), cand[sp[0]:sp[1]], o); tag = '   <<<'
    print('  %-18s %s%s' % (o.split('/')[-2], line, tag))
if best[1]:
    open(os.path.join(SP, 'best.c'), 'w').write(best[1])
    print('BEST %d rows (was %d) from %s' % (best[0], cur, best[2]))
    print('saved: %s' % os.path.join(SP, 'best.c'))
else:
    print('no find beats the park (%d rows)' % cur)
