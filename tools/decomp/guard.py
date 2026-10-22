#!/usr/bin/env python3
"""guard.py -- every function the splat asm knows about must still be either
an INCLUDE_ASM stub or a definition in csrc/."""
import glob, re, sys, os
SP = os.environ.get('AZURE_SCRATCH', '/tmp/azure-decomp')
os.makedirs(SP, exist_ok=True)
PROJ = os.environ.get('AZURE_ROOT',
    os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
known = set()
for f in glob.glob(PROJ + '/asm/nonmatchings/*/*.s'):
    m = re.search(r'nonmatching (\w+),', open(f).read())
    if m:
        known.add(m.group(1))
have = set()
for f in glob.glob(PROJ + '/csrc/*.c'):
    t = open(f).read()
    have |= set(re.findall(r'INCLUDE_ASM\("[^"]+", (\w+)\)', t))
    have |= set(re.findall(r'^[A-Za-z_][\w \*]*?\b(func_[0-9A-Fa-f]+)\s*\([^;]*\)\s*\{', t, re.M))
missing = sorted(known - have)
stubs = sum(len(re.findall(r'INCLUDE_ASM', open(f).read())) for f in glob.glob(PROJ + '/csrc/*.c'))
print('%d known, %d accounted for, %d still asm stubs' % (len(known), len(have & known), stubs))
if missing:
    print('MISSING:', missing)
    sys.exit(1)
