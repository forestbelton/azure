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
    # The parameter-list class MUST be lazy. Greedy `[^;]*` crosses newlines,
    # so a run of consecutive bodyless definitions -- which splat emits for
    # trivially empty functions, e.g. two `void func_X(void) {}` in a row --
    # collapses into ONE match and only the first name is captured. That made
    # guard.py report present functions as MISSING, which is the opposite of
    # its job. Lazy matching stops at the nearest `) {` and still backtracks
    # correctly through a nested `(...)` in a parameter list.
    have |= set(re.findall(r'^[A-Za-z_][\w \*]*?\b(func_[0-9A-Fa-f]+)\s*\([^;]*?\)\s*\{', t, re.M))
    # A definition may carry an __asm__ label, so the C name is NOT the
    # assembler name: `void func_Xd(u16) __asm__("func_X"); void func_Xd(...)
    # { ... }`. That is how a unit gives one function a narrow parameter while
    # its own callers keep a wide prototype (matching-levers section 7), and
    # without this the function reads as MISSING even though it is compiled.
    for name, label in re.findall(
            r'^[A-Za-z_][\w \*]*?\b(\w+)\s*\([^;]*?\)\s*'
            r'__asm__\s*\(\s*"(\w+)"\s*\)\s*;', t, re.M):
        if re.search(r'^[A-Za-z_][\w \*]*?\b' + name + r'\s*\([^;]*?\)\s*\{',
                     t, re.M):
            have.add(label)
missing = sorted(known - have)
# Count macro INVOCATIONS, not the bare word: `INCLUDE_ASM` also appears in
# prose inside csrc comments, which silently inflated this project's headline
# stub count.
stubs = sum(len(re.findall(r'INCLUDE_ASM\("[^"]+",\s*\w+\)', open(f).read()))
            for f in glob.glob(PROJ + '/csrc/*.c'))
print('%d known, %d accounted for, %d still asm stubs' % (len(known), len(have & known), stubs))
if missing:
    print('MISSING:', missing)
    sys.exit(1)
