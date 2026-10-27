#!/usr/bin/env python3
"""triage.py -- rank the remaining INCLUDE_ASM stubs by how cheap they look.

Working the carved units strictly cheapest-first by WORD COUNT is the wrong
order, because word count is a poor proxy for effort. Measured over the
functions landed on 2026-10-26: call sequences and struct setups went in on the
first or second attempt regardless of size (func_80048E00 at 57 words, first
try; func_8004878C at 57, two), while every function parked at the exact word
count was either a loop whose address arithmetic loop.c strength-reduced
differently (func_80054104, func_80059DAC) or a register permutation in a
branch-heavy body (func_8005500C, func_8003AB44, func_80055D84).

So this scores the SHAPE of the target listing instead:

  loops      a backward branch. The expensive feature by a wide margin -- it
             brings loop.c's invariant motion and strength reduction in, and
             those decisions are not reachable from how a subscript is spelled.
  jtbl       dispatches through a jump table whose .rodata is unplaced, so it
             cannot land at all yet (see the region map in
             docs/open-matching-problems.md).
  muldiv     a multiply or divide, usually meaning a constant-division magic to
             recover with magic.py, and signedness to get right.
  calls      `jal`s. These make a function LOOK big and are the cheapest thing
             in it: argument setup is mechanical.
  stores     `sb`/`sh`/`sw`. Struct setup, also mechanical.
  spwrite    a write to $sp that is not `addiu $sp,$sp,N`: a STACK SWITCH,
             which plain C cannot express. func_8004CAA0 moves $sp into the PSX
             scratchpad at 0x1F8003FC around a call and scores cheap on every
             other feature; it wants inline asm or a decomp device.
  stackarg   reads of an incoming argument slot, i.e. five or more parameters.
             Cheap, but see the narrow-parameter entry in matching-levers 7
             before declaring their types.

`EASY` means no loop, no jump table, no multiply and no stack switch; those are the rows to
take first. The score is only a sort key -- it decides nothing, and a listing
still has to be read.

    python3 tools/decomp/triage.py                 # all remaining stubs, best first
    python3 tools/decomp/triage.py --easy --fresh  # the rows to actually take
    python3 tools/decomp/triage.py --unit unk_72A8

`--fresh` drops anything carved-sweep.md or docs/near-misses/ already has a
measurement for, because an attempt somebody already spent is not cheap. The
two parked rows that say "not attempted" are kept and marked `listed`.
"""
import os
import re
import glob
import sys

import asmlib
from asmlib import BRANCHES, LOADS, STORES, REGS

def units():
    """Every csrc unit that still has an INCLUDE_ASM, newest cut first.

    This was a hardcoded tuple and went stale the moment the carved unit
    unk_1C540 was cut into five pieces for the jump-table placements: the
    functions that moved into unk_25944, unk_264E0, unk_2879C and unk_2CDC0
    simply stopped being triaged, silently. Read csrc/ instead -- it is the
    same source progress.py and guard.py use, so the three cannot disagree
    about what exists.
    """
    out = []
    for f in sorted(glob.glob(os.path.join(asmlib.PROJ, 'csrc', '*.c'))):
        t = open(f, encoding='utf-8', errors='replace').read()
        if re.search(r'INCLUDE_ASM\("[^"]+",\s*\w+\)', t):
            out.append(os.path.basename(f)[:-2])
    return out


def already_tried():
    """Names that carved-sweep.md or a near-miss file already measures.

    A row with a measurement behind it is NOT a cheap row even when its shape
    looks cheap -- somebody has already spent the attempt. Two rows of the
    parked table say "not attempted"/"not measured in isolation", so those are
    reported as `listed` rather than `parked`.
    """
    tried, listed = set(), set()
    for rel in ('docs/carved-sweep.md',):
        path = os.path.join(asmlib.PROJ, rel)
        for line in open(path, encoding='utf-8'):
            names = re.findall(r'func_[0-9A-Fa-f]{8}', line)
            if not names:
                continue
            if 'not attempted' in line or 'not measured' in line:
                listed.update(names)
            else:
                tried.update(names)
    d = os.path.join(asmlib.PROJ, 'docs/near-misses')
    if os.path.isdir(d):
        for f in os.listdir(d):
            tried.update(re.findall(r'func_[0-9A-Fa-f]{8}', f))
    return tried - listed, listed


def stubs(unit):
    path = os.path.join(asmlib.PROJ, 'csrc', unit + '.c')
    if not os.path.exists(path):
        return []
    pat = re.compile(r'^INCLUDE_ASM\("asm/nonmatchings/[^"]*",\s*(func_[0-9A-Fa-f]+)\);',
                     re.M)
    return pat.findall(open(path, encoding='utf-8').read())


def features(unit, func):
    try:
        words = asmlib.target_words(unit, func)
    except Exception:
        return None
    ins = asmlib.decode_target(words)
    frame = 0
    for i in ins[:4]:
        if i.op == 'addiu' and REGS[i.rt] == 'sp' and REGS[i.rs] == 'sp' and i.simm < 0:
            frame = -i.simm
    f = {
        'words': len(ins),
        'loops': sum(1 for i in ins if i.op in BRANCHES and i.simm < 0),
        'calls': sum(1 for i in ins if i.op == 'jal'),
        'stores': sum(1 for i in ins if i.op in STORES and REGS[i.rs] != 'sp'),
        'muldiv': sum(1 for i in ins if i.op in ('mult', 'multu', 'div', 'divu')),
        'branches': sum(1 for i in ins if i.op in BRANCHES),
        # an incoming argument slot is at or above frame+0x10
        'stackarg': sum(1 for i in ins if i.op in LOADS and REGS[i.rs] == 'sp'
                        and i.simm >= frame + 0x10),
        'jtbl': any('jtbl_' in t for _, t in words),
        # A write to $sp that is not `addiu $sp,$sp,N` is a STACK SWITCH, which
        # plain C cannot express at all -- func_8004CAA0 moves $sp into the PSX
        # scratchpad at 0x1F8003FC around a call, and scored EASY on every
        # other feature. Those want inline asm or a decomp device, so they are
        # not cheap however small they are.
        'spwrite': sum(1 for i in ins
                       if REGS[i.rd] == 'sp' and i.op in ('addu', 'add', 'or')
                       or (i.op == 'addiu' and REGS[i.rt] == 'sp'
                           and REGS[i.rs] != 'sp')),
    }
    return f


def main():
    only_easy = '--easy' in sys.argv
    only_fresh = '--fresh' in sys.argv
    tried, listed = already_tried()
    unit_arg = None
    for a in sys.argv[1:]:
        if a.startswith('--unit'):
            unit_arg = a.split('=', 1)[1] if '=' in a else None
    if unit_arg is None and '--unit' in sys.argv:
        i = sys.argv.index('--unit')
        if i + 1 < len(sys.argv):
            unit_arg = sys.argv[i + 1]

    rows = []
    for unit in units():
        if unit_arg and unit != unit_arg:
            continue
        for func in stubs(unit):
            f = features(unit, func)
            if f is None:
                continue
            easy = (not f['loops'] and not f['jtbl'] and not f['muldiv']
                    and not f['spwrite'])
            state = 'parked' if func in tried else ('listed' if func in listed else '')
            if only_fresh and state == 'parked':
                continue
            # cheapest-looking first: shape dominates, size only breaks ties
            score = (f['loops'] * 100 + f['jtbl'] * 1000 + f['muldiv'] * 20
                     + f['branches'] * 3 + f['words']
                     + f['spwrite'] * 400)
            score += 500 if state == 'parked' else 0
            rows.append((score, unit, func, f, easy, state))
    rows.sort()

    print('%-14s %-16s %5s %5s %5s %5s %6s %4s %4s %-14s %s'
          % ('unit', 'func', 'words', 'loops', 'brch', 'calls', 'stores',
             'm/d', 'arg', 'verdict', 'state'))
    n_easy = 0
    for score, unit, func, f, easy, state in rows:
        if easy:
            n_easy += 1
        elif only_easy:
            continue
        print('%-14s %-16s %5d %5d %5d %5d %6d %4d %4d %-14s %s'
              % (unit, func, f['words'], f['loops'], f['branches'], f['calls'],
                 f['stores'], f['muldiv'], f['stackarg'],
                 'EASY' if easy else ('JTBL-BLOCKED' if f['jtbl'] else
                                      ('STACK-SWITCH' if f['spwrite'] else
                                       ('loop' if f['loops'] else 'muldiv'))),
                 state))
    print('\n%d stub(s), %d look EASY (no loop, no jump table, no multiply)'
          % (len(rows), n_easy))
    return 0


if __name__ == '__main__':
    sys.exit(main())
