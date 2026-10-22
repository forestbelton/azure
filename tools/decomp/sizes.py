#!/usr/bin/env python3
"""sizes.py -- report every C function whose compiled size differs from the
target. Sizes come from the built objects, targets from the splat asm."""
import glob, re, subprocess, os
SP = os.environ.get('AZURE_SCRATCH', '/tmp/azure-decomp')
os.makedirs(SP, exist_ok=True)
PROJ = os.environ.get('AZURE_ROOT',
    os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
want = {}
for f in glob.glob(PROJ + '/asm/nonmatchings/*/*.s'):
    t = open(f).read()
    m = re.search(r'nonmatching (\w+), (0x[0-9A-Fa-f]+)', t)
    if m:
        want[m.group(1)] = int(m.group(2), 16)
bad = []
for o in glob.glob(PROJ + '/build/csrc/*.o'):
    out = subprocess.run(['mips64-linux-gnu-objdump', '-t', o],
                         capture_output=True, text=True).stdout
    for l in out.split('\n'):
        p = l.split()
        if len(p) >= 6 and p[2] == 'F' and p[5] in want:
            got = int(p[4], 16)
            if got != want[p[5]]:
                bad.append((p[5], want[p[5]], got, o.split('/')[-1]))
if not bad:
    print('all compiled function sizes match their targets')
for name, w, g, o in sorted(bad):
    print('%s  %-10s want 0x%X  got 0x%X  (%+d instructions)'
          % (name, o, w, g, (g - w) // 4))
