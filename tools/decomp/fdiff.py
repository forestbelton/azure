#!/usr/bin/env python3
"""fdiff.py FUNC... -- side-by-side disassembly of built vs reference bytes."""
import glob, re, subprocess, sys, os
SP = os.environ.get('AZURE_SCRATCH', '/tmp/azure-decomp')
os.makedirs(SP, exist_ok=True)
PROJ = os.environ.get('AZURE_ROOT',
    os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
TMP = os.path.join(SP, '.fd')

def dis(data, vram):
    open(TMP, 'wb').write(data)
    out = subprocess.run(['mips64-linux-gnu-objdump', '-D', '-b', 'binary',
        '-m', 'mips:3000', '-EL', '--adjust-vma', hex(vram), TMP],
        capture_output=True, text=True).stdout
    rows = []
    for l in out.split('\n'):
        m = re.match(r'\s*([0-9a-f]+):\t[0-9a-f ]+\t(.*)', l)
        if m:
            rows.append(re.sub(r'\s+', ' ', m.group(2).strip()))
    return rows

ref = open(PROJ + '/data/slus_006.14', 'rb').read()
built = open(PROJ + '/build/slus_006.14', 'rb').read()
for func in sys.argv[1:]:
    f = glob.glob('%s/asm/nonmatchings/*/%s.s' % (PROJ, func))
    if not f:
        print('%s: no asm' % func); continue
    t = open(f[0]).read()
    size = int(re.search(r'nonmatching %s, (0x[0-9A-Fa-f]+)' % func, t).group(1), 16)
    m = re.search(r'/\* ([0-9A-F]+) ([0-9A-F]{8})', t)
    off, vram = int(m.group(1), 16), int(m.group(2), 16)
    rb, bb = ref[off:off+size], built[off:off+size]
    if rb == bb:
        print('=== %s  MATCH' % func); continue
    a, b = dis(bb, vram), dis(rb, vram)
    bad = sum(1 for x, y in zip(a, b) if x != y) + abs(len(a) - len(b))
    print('=== %s  DIFF  (%d/%d instructions)' % (func, bad, len(b)))
    for i in range(max(len(a), len(b))):
        x = a[i] if i < len(a) else ''
        y = b[i] if i < len(b) else ''
        print('  %-36s %s %s' % (x, ' ' if x == y else 'X', y))
