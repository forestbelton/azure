#!/usr/bin/env python3
"""batcher.py UNIT [--words N] [--skip N] [--take N] -- partition the REMAINING
stubs into coherent agent-sized groups.

nearsibs.py only sees functions that have a near-sibling, which ran out at
~157 pairs with ~690 stubs still left. This covers everything remaining, and
groups on the three things that actually predicted a cheap landing in this
sweep:

  1. a shared callee or global, weighted by RARITY (a callee the whole unit
     calls means nothing; one shared with two means a great deal);
  2. what a function INSTALLS, and who installs IT -- the +0x50/unk_00/unk_10
     slot chains are how this unit's families are really related, and the
     callee screen cannot see them;
  3. ADDRESS LOCALITY, because a stub's immediate neighbours in the file were
     the right template more often than any score, and the file is ordered by
     address.

Greedy agglomeration from the largest unplaced stub, pulling in the
highest-affinity remaining stubs until the group hits --words.
"""
import collections, os, re, sys

PROJ = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

def main():
    unit = sys.argv[1]
    want = int(sys.argv[sys.argv.index('--words') + 1]) if '--words' in sys.argv else 200
    skip = int(sys.argv[sys.argv.index('--skip') + 1]) if '--skip' in sys.argv else 0
    take = int(sys.argv[sys.argv.index('--take') + 1]) if '--take' in sys.argv else 8
    D = os.path.join(PROJ, 'asm/nonmatchings', unit)
    src = open(os.path.join(PROJ, 'csrc', unit + '.c')).read()

    stubs = set(re.findall(r'INCLUDE_ASM\("[^"]*", *(func_[0-9A-Fa-f]{8})\)', src))
    parked = set()
    pk = os.path.join(PROJ, 'docs/near-misses')
    if os.path.isdir(pk):
        parked = {f[:-2] for f in os.listdir(pk) if f.startswith('func_') and f.endswith('.c')}
    stubs -= parked

    info = {}
    for f in sorted(stubs):
        p = os.path.join(D, f + '.s')
        if not os.path.exists(p): continue
        t = open(p).read()
        m = re.search(r'nonmatching \S+, (0x[0-9A-F]+)', t)
        if not m: continue
        info[f] = dict(
            w=int(m.group(1), 16) // 4,
            cal=set(re.findall(r'jal\s+(\w+)', t)),
            ins=set(re.findall(r'%hi\((func_[0-9A-Fa-f]+)\)', t)),
            gl=set(re.findall(r'%hi\((D_[0-9A-F]+)\)', t)),
            jt=len(re.findall(r'jr\s+\$(?!ra)', t)),
            rod=1 if ('.section .rodata' in t or re.search(r'^dlabel D_', t, re.M)) else 0,
            addr=int(f[5:], 16))

    freq = collections.Counter()
    for d in info.values():
        for c in d['cal']: freq[c] += 1
        for g in d['gl']: freq[g] += 1

    def aff(a, b):
        A, B = info[a], info[b]
        s = 0.0
        for c in A['cal'] & B['cal']: s += 1.0 / freq[c]
        for g in A['gl'] & B['gl']: s += 2.0 / freq[g]
        # slot chains: either installs the other, or they install the same thing
        if b in A['ins'] or a in B['ins']: s += 4.0
        for h in A['ins'] & B['ins']: s += 3.0
        # address locality, in units of ~0x400 bytes
        d = abs(A['addr'] - B['addr'])
        if d < 0x2000: s += 1.5 * (0x2000 - d) / 8192.0
        return s

    left = dict(info)
    groups = []
    while left:
        seed = max(left, key=lambda f: left[f]['w'])
        g = [seed]; w = left[seed]['w']; del left[seed]
        while left and w < want:
            best = max(left, key=lambda f: (max(aff(f, x) for x in g), -abs(left[f]['addr'] - info[g[0]]['addr'])))
            if max(aff(best, x) for x in g) <= 0.05 and len(g) > 1: break
            g.append(best); w += left[best]['w']; del left[best]
        groups.append((w, g))

    groups.sort(key=lambda t: -t[0])
    print("%d stubs, %d words, %d groups (target %dw)"
          % (len(info), sum(d['w'] for d in info.values()), len(groups), want))
    for i, (w, g) in enumerate(groups[skip:skip + take], 1 + skip):
        haz = [f for f in g if info[f]['jt'] or info[f]['rod']]
        print("\nGROUP %d  %d words  %d members%s" % (i, w, len(g),
              '   *** HAZARD: %s' % ','.join(haz) if haz else ''))
        print("  " + ' '.join('%s(%d)' % (f, info[f]['w']) for f in g))

if __name__ == '__main__':
    sys.exit(main())
