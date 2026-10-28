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
    # AN OLD-STYLE (K&R) DEFINITION puts its parameter DECLARATIONS between the
    # `)` and the `{`, so the pattern above -- which wants `) {` -- misses it
    # and the function reads as MISSING even though it is compiled. This
    # project uses that form deliberately: it is how a unit narrows a
    # parameter without putting a narrow prototype in scope for its callers
    # (matching-levers section 7, and func_8005C2C0 is the case that exposed
    # this). Accept a run of `type name;` declarations before the brace.
    have |= set(re.findall(
        r'^[A-Za-z_][\w \*]*?\b(func_[0-9A-Fa-f]+)\s*\([\w\s,\*]*?\)\s*'
        r'(?:[A-Za-z_][\w \*]*[\w\*]\s*[\w\*\[\] ,]*;\s*)+\{', t, re.M))
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

# docs/carved-sweep.md's rows are the measurement record for every function
# that has no park file, and a row for a function that has since LANDED is
# worse than no row: it sends the next session hunting for words that are
# already paid. Three such rows were found by hand on 2026-10-27, so check it.
stub_names = set()
for f in glob.glob(PROJ + '/csrc/*.c'):
    stub_names |= set(re.findall(r'INCLUDE_ASM\("[^"]+",\s*(\w+)\)',
                                 open(f).read()))
try:
    sweep = open(PROJ + '/docs/carved-sweep.md').read()
except OSError:
    sweep = ''
stale = [m.group(1) for m in
         re.finditer(r'^\| (func_\w+) \| \w+ \| \*{0,2}\d+/\d+ words?, \d+ DIFF',
                     sweep, re.M)
         if m.group(1) not in stub_names]
if stale:
    print('STALE carved-sweep.md rows (these functions have LANDED):',
          ', '.join(sorted(stale)))
if missing:
    print('MISSING:', missing)
    sys.exit(1)
