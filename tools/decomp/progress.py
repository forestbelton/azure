#!/usr/bin/env python3
"""progress.py -- how far each translation unit has got, by function and by word.

`guard.py` answers one question ("is every function splat knows about still
accounted for?") and prints one number. This answers the other one: for each
file in csrc/, how many of its functions are C, how many are still
INCLUDE_ASM, how many of those have a park file with measurements, and what
share of the unit's WORDS is done -- which is the figure that moves when a
600-word function lands and the function count barely notices.

  progress.py                 every unit, plus a TOTAL row
  progress.py unk_1C540 ...   those units' remaining stubs, smallest first
  progress.py --parked        every park file, closest first

The function names come from csrc/*.c -- INCLUDE_ASM invocations and
definitions, with guard.py's own regexes, so the two tools cannot disagree
about what exists. The SIZES come from `objdump -t` on build/csrc/<unit>.o,
which carries every function in the unit including the INCLUDE_ASM'd ones,
since the asm is assembled into the same object. That means the word columns
need a build: without one they read `-`, and after a source edit they are as
stale as build/. The asm/nonmatchings headers are the fallback, and they only
ever cover the stubs.
"""
import glob, os, re, subprocess, sys

PROJ = os.environ.get('AZURE_ROOT',
    os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
OBJDUMP = os.environ.get('OBJDUMP', 'mips64-linux-gnu-objdump')

STUB_RE = re.compile(r'INCLUDE_ASM\("[^"]+",\s*(\w+)\)')
# The parameter list must be matched lazily -- see the note in guard.py, whose
# regex this is. Greedy matching swallows a run of bodyless definitions whole
# and reports all but the first as missing.
DEF_RE = re.compile(r'^[A-Za-z_][\w \*]*?\b(func_[0-9A-Fa-f]+)\s*\([^;]*?\)\s*\{', re.M)


def units():
    """unit -> (C function names, INCLUDE_ASM names), in csrc order."""
    out = {}
    for f in sorted(glob.glob(PROJ + '/csrc/*.c')):
        t = open(f).read()
        stubs = STUB_RE.findall(t)
        done = [n for n in DEF_RE.findall(t) if n not in stubs]
        out[os.path.basename(f)[:-2]] = (done, stubs)
    return out


def sizes(unit):
    """name -> size in bytes for one unit, from its object or from the asm."""
    o = '%s/build/csrc/%s.o' % (PROJ, unit)
    if os.path.exists(o):
        try:
            out = subprocess.run([OBJDUMP, '-t', o], capture_output=True,
                                 text=True).stdout
        except FileNotFoundError:
            out = ''
        if out:
            got = {}
            for l in out.split('\n'):
                p = l.split()
                # addr flags 'F' '.text' size name -- the size field is the
                # one before the name, and a symbol with no size (an alias,
                # or a label) must not overwrite a real one.
                if len(p) >= 2 and ' F ' in l and '.text' in l:
                    n, sz = p[-1], int(p[-2], 16)
                    if sz or n not in got:
                        got[n] = sz
            if got:
                return got
    got = {}
    for f in glob.glob(PROJ + '/asm/nonmatchings/*/*.s'):
        m = re.search(r'nonmatching (\w+), (0x[0-9A-Fa-f]+)', open(f).read())
        if m:
            got[m.group(1)] = int(m.group(2), 16)
    return got


def parks():
    """func -> (built, target, diff), or None when the first line says nothing."""
    out = {}
    for f in glob.glob(PROJ + '/docs/near-misses/func_*.c'):
        m = re.search(r'(\d+) of (\d+) words, (\d+) DIFF', open(f).readline())
        out[os.path.basename(f)[:-2]] = (int(m.group(1)), int(m.group(2)),
                                         int(m.group(3))) if m else None
    return out


def table(us, PARK):
    rows = []
    unknown = False
    for unit, (done, stubs) in us.items():
        sz = sizes(unit)
        wd = sum(sz.get(n, 0) for n in done) // 4
        ws = sum(sz.get(n, 0) for n in stubs) // 4
        if not sz:
            unknown = True
        rows.append((unit, len(done) + len(stubs), len(done), len(stubs),
                     len([n for n in stubs if n in PARK]), wd, wd + ws))
    rows.sort(key=lambda r: (-r[3], -r[6]))
    w = max(len(r[0]) for r in rows)
    fmt = '%-*s  %5s %5s %5s %6s  %7s %7s %6s'
    print(fmt % (w, 'unit', 'funcs', 'C', 'stub', 'parked', 'words', 'of', 'done'))
    tot = [0] * 6
    for r in rows:
        tot = [a + b for a, b in zip(tot, r[1:])]
        pct = '%5.1f%%' % (100.0 * r[5] / r[6]) if r[6] else '     -'
        print('%-*s  %5d %5d %5d %6d  %7s %7s %s' %
              (w, r[0], r[1], r[2], r[3], r[4],
               r[5] or '-', r[6] or '-', pct))
    pct = '%5.1f%%' % (100.0 * tot[4] / tot[5]) if tot[5] else '     -'
    print('%-*s  %5d %5d %5d %6d  %7d %7d %s' %
          (w, 'TOTAL', tot[0], tot[1], tot[2], tot[3], tot[4], tot[5], pct))
    if unknown:
        print('\nsome units have no build/csrc/*.o; run make for the word columns')


def listing(us, PARK, wanted):
    for unit in wanted:
        if unit not in us:
            print('no such unit: %s' % unit, file=sys.stderr)
            continue
        stubs = us[unit][1]
        sz = sizes(unit)
        print('%s -- %d stubs, %d words still asm' %
              (unit, len(stubs), sum(sz.get(n, 0) for n in stubs) // 4))
        for n in sorted(stubs, key=lambda n: sz.get(n, 0)):
            if n not in PARK:
                state = ''
            elif PARK[n] is None:
                state = '  parked'
            else:
                state = '  parked %d/%d words, %d DIFF' % PARK[n]
            print('  %-18s %5d words%s' % (n, sz.get(n, 0) // 4, state))


def parked_listing(PARK):
    rows = [(n,) + p for n, p in PARK.items() if p]
    rows.sort(key=lambda r: (r[3], r[2]))
    for n, built, target, diff in rows:
        flag = '' if built == target else '  (%+d words)' % (built - target)
        print('%-18s %5d of %-5d words  %4d DIFF%s' % (n, built, target, diff, flag))
    blank = sorted(n for n, p in PARK.items() if not p)
    if blank:
        print('\nno measurement in the first line of: %s' % ' '.join(blank))


if __name__ == '__main__':
    args = [a for a in sys.argv[1:] if not a.startswith('-')]
    us, PARK = units(), parks()
    if '--parked' in sys.argv:
        parked_listing(PARK)
    elif args:
        listing(us, PARK, args)
    else:
        table(us, PARK)
