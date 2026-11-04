#!/usr/bin/env python3
"""seqcheck.py UNIT_FILE FUNC... -- decode the words `ovlverify` MASKS.

`ovlverify` masks every relocated word, so a wrong `jal` target, a wrong
`%hi`/`%lo` symbol and a wrong intra-function `j` destination are all
invisible to it. Two adjacent calls with identical delay slots are
byte-indistinguishable in the masked run, and that has fired for real
(func_800B2BA0). This is the other half of the check, and it is ORDERED: a
multiset comparison of the symbols cannot see such a swap.

For each function it prints `--raw` differing, `relocdump.py`'s total, the
listing's masked-word count, how many of those are on a FUNCTION symbol, and
two verdicts:

    arith  raw differing == relocdump's own total.  Do NOT add the
           intra-function `j` words separately -- relocdump already counts
           each one as R_MIPS_26 against .text, and that sum double-counts.
    seq    the ORDERED sequence of (jal symbol, %hi/%lo symbol, intra-function
           j offset) off the listing equals the candidate's.

One deviation passes `seq` with a note: a CONSTANT ADDRESS emits no
relocation, so splat's listing shows `%hi`/`%lo` where the candidate has
nothing and the candidate's sequence is a SUBSEQUENCE of the target's. That is
benign when every target-only entry is on a DATA symbol and the arithmetic
closes, because `raw == relocdump total` means every word the candidate does
not relocate matched byte for byte. A missing `%hi`/`%lo` on a FUNCTION symbol
is a build break, not this class, and still fails.

A symbol MISMATCH (not a missing entry) can also be legitimate -- a candidate
may relocate against a different symbol plus an addend, `D_80082660` + 8 where
splat prints `D_80082668` -- but that one is not auto-classified: decode the
resolved immediate by hand and let `make build/town.bin` settle it.

    python3 tools/decomp/seqcheck.py scratch/x1_unit.c func_80095094 func_80094F58

Exit 1 if any check fails.
"""

import glob
import os
import re
import subprocess
import sys

PROJ = os.environ.get("AZURE_ROOT", os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))


def target_seq(func):
    # Find the listing by GLOB rather than hard-coding one overlay: the town
    # overlay alone is carved into three translation units, and the same check
    # has to work for any of them and for the resident units.
    hits = glob.glob(os.path.join(PROJ, 'asm/nonmatchings/*', func + '.s'))
    if len(hits) != 1:
        raise SystemExit('%s: %d listings matched asm/nonmatchings/*/%s.s'
                         % (func, len(hits), func))
    path = hits[0]
    base = int(func.split('_')[1], 16)
    out = []
    for line in open(path):
        m = re.match(r'\s*/\* \w+ ([0-9A-F]{8}) \w{8} \*/\s+(\S+)\s*(.*)', line)
        if not m:
            continue
        addr, op, rest = int(m.group(1), 16), m.group(2), m.group(3)
        for mm in re.finditer(r'%(hi|lo)\((\w+)\)', rest):
            out.append('%s %s' % ('HI16' if mm.group(1) == 'hi' else 'LO16', mm.group(2)))
        if op == 'jal':
            out.append('jal %s' % rest.strip())
        elif op == 'j':
            mm = re.search(r'\.L([0-9A-F]+)', rest)
            if mm:
                out.append('j 0x%x@0x%x' % (int(mm.group(1), 16) - base, addr - base))
    return out


def cand_seq(unit, func):
    txt = subprocess.run([sys.executable, os.path.join(PROJ, 'tools/decomp/relocdump.py'),
                          unit, func], cwd=PROJ, capture_output=True, text=True).stdout
    out, last, total = [], None, 0
    # the section offset of this function's first word, to turn an encoded
    # j target into a function-relative offset
    first = None
    for L in txt.splitlines():
        m = re.match(r'\s*([0-9a-f]+) ([0-9a-f]{8}) (\S+)(.*)', L)
        if m:
            last = m
            if first is None:
                first = int(m.group(1), 16)
        if 'RELOC' in L:
            total += 1
            parts = L.split()
            typ, sym = parts[1], parts[2]
            if last is not None and last.group(3) == 'j':
                off = int(last.group(1), 16)
                # objdump renders the target as <func+0xNNN>; that is the
                # function-relative offset, and it is the only place the
                # function's own section base is visible here.
                mm = re.search(r'<%s\+0x([0-9a-f]+)>' % func, last.group(4))
                if mm:
                    tgt = int(mm.group(1), 16)
                elif re.search(r'<%s>' % func, last.group(4)):
                    tgt = 0
                else:
                    tgt = -1   # a j OUT of the function: should not happen
                out.append('j 0x%x@0x%x' % (tgt, off))
            elif last is not None and last.group(3) == 'jal':
                out.append('jal %s' % sym)
            else:
                out.append('%s %s' % (typ.replace('R_MIPS_', ''), sym))
    return out, total


def main():
    unit, funcs = sys.argv[1], sys.argv[2:]
    bad = 0
    for f in funcs:
        raw = subprocess.run([sys.executable, os.path.join(PROJ, 'tools/decomp/ovlverify.py'),
                              '--raw', unit, f], cwd=PROJ, capture_output=True, text=True).stdout
        m = re.search(r'(\d+) differing', raw)
        rawn = int(m.group(1)) if m else -1
        t = target_seq(f)
        c, total = cand_seq(unit, f)
        fn = len([x for x in t if re.match(r'(HI16|LO16) func_', x)])
        ok_arith = (rawn == total)
        ok_seq = (t == c)
        note = ''
        if not ok_seq and ok_arith:
            # A CONSTANT ADDRESS emits no relocation, so splat's listing shows
            # %hi/%lo where the candidate has none and the candidate's
            # sequence comes out a SUBSEQUENCE of the target's.  That is
            # benign, and the raw comparison is what proves it: raw ==
            # relocdump's total means every word the candidate does NOT
            # relocate matched byte for byte, including those words.  It is
            # only benign when every target-only entry is on a DATA symbol --
            # a missing %hi/%lo on a FUNCTION symbol is a build break.
            i, missing = 0, []
            for e in t:
                if i < len(c) and c[i] == e:
                    i += 1
                else:
                    missing.append(e)
            if i == len(c) and missing and all(
                    re.match(r'(HI16|LO16) D_', e) for e in missing):
                ok_seq = True
                note = ('  [%d constant-address word(s): %s -- no relocation, '
                        'verified by the raw comparison]'
                        % (len(missing), ', '.join(missing)))
        print('%-16s raw=%-3d relocdump=%-3d masked_words=%-3d on_function=%-2d  arith=%s  seq=%s%s'
              % (f, rawn, total, len(t), fn, 'OK' if ok_arith else 'FAIL',
                 'OK' if ok_seq else 'FAIL', note))
        if not (ok_arith and ok_seq):
            bad += 1
            for i, (a, b) in enumerate(zip(t, c)):
                if a != b:
                    print('    first mismatch at %d: target %r  candidate %r' % (i, a, b))
                    break
            if len(t) != len(c):
                print('    length target %d candidate %d' % (len(t), len(c)))
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
