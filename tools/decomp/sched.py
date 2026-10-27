#!/usr/bin/env python3
"""sched.py -- read cc1psx's instruction-scheduler decisions for one function.

`tools/decomp/dump.sh` already collects all sixteen of cc1psx's RTL dumps;
this reads the two scheduler ones out of that collection and prints the part
for one function, so that an insn number in a ready list can be turned back
into an instruction.

    sched2  after reload, with HARD registers -- the pass that interleaves the
            prologue saves with the body, so this is the one matching wants
    sched   before register allocation

Each dump prints, per basic block: every insn's `priority` and `ref_count`;
the ready list at each cycle as `;; ready list at T-N:` with the reordering
that follows it; every hazard decision (`;; blocking insn N for M cycles`,
`;; insn N has a greater potential hazard`, `;; launching N before M`); and
then the scheduled RTL.

WHY THIS EXISTS: "sched's tie-break is INSN_LUID" is the THIRD key, not the
first, and several write-ups in this repo leaned on it. The order the keys
actually apply in -- with gcc 2.7.2's sched.c line numbers -- is in section 8
of docs/matching-levers.md. Two things that dump reading settles immediately:

  * which insns share a priority, because `schedule_select` only ever chooses
    WITHIN a priority group -- and priority is the longest path from the block
    START, so the whole top-of-function region is priority 1 and the choice
    there is `potential_hazard`, never the dependence graph (func_8005947C is
    priority 1 throughout; func_8003719C runs 1 to 11, all of its entry region
    at 1);
  * which insn number is which instruction, via `--insn`.

Scheduling runs BACKWARD: `T-N` counts cycles from the block end, so the insn
chosen at the LARGEST T is the one emitted first.

    python3 tools/decomp/sched.py cand.c func_8003719C
    python3 tools/decomp/sched.py cand.c func_8003719C --sched1
    python3 tools/decomp/sched.py cand.c func_8003719C --insn 27
    python3 tools/decomp/sched.py cand.c func_8003719C --rtl

`--keep` skips the recompile and reads the dumps already in the scratch
directory, which is what to use when stepping through several insn numbers.
"""
import os
import re
import subprocess
import sys

import asmlib


def collect(src, keep=False):
    """Run dump.sh for `src` unless --keep; return its dump directory."""
    d = os.path.join(asmlib.SP, 'dumps', os.path.splitext(os.path.basename(src))[0])
    if keep and os.path.isdir(d):
        return d
    r = subprocess.run([os.path.join(asmlib.PROJ, 'tools/decomp/dump.sh'), src],
                       cwd=asmlib.PROJ, capture_output=True, text=True)
    if not os.path.isdir(d):
        raise asmlib.CompileError(r.stdout + r.stderr)
    return d


def section(text, func):
    """The dump text for one function, between its header and the next."""
    starts = [(m.start(), m.group(1))
              for m in re.finditer(r'^;; Function (\S+)', text, re.M)]
    for k, (pos, name) in enumerate(starts):
        if name == func:
            end = starts[k + 1][0] if k + 1 < len(starts) else len(text)
            return text[pos:end]
    raise SystemExit('sched.py: no `;; Function %s` in the dump (it holds: %s)'
                     % (func, ', '.join(n for _, n in starts[:8]) or 'nothing'))


def split_rtl(sec):
    """(decisions, rtl): the `;;` trace, and the printed RTL after it."""
    m = re.search(r'^\(note ', sec, re.M)
    return (sec[:m.start()], sec[m.start():]) if m else (sec, '')


def one_insn(rtl, n):
    """The RTL for insn N as printed, paren-balanced."""
    m = re.search(r'^\((?:insn|jump_insn|call_insn)(?::\w+)? %d ' % n, rtl, re.M)
    if not m:
        return None
    depth = 0
    for j in range(m.start(), len(rtl)):
        if rtl[j] == '(':
            depth += 1
        elif rtl[j] == ')':
            depth -= 1
            if depth == 0:
                return rtl[m.start():j + 1]
    return rtl[m.start():]


def main():
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    if len(args) < 2:
        raise SystemExit('usage: sched.py <file.c> <func> '
                         '[--sched1] [--insn N] [--rtl] [--keep]')
    src, func = args[0], args[1]
    name = 'sched' if '--sched1' in sys.argv else 'sched2'
    path = os.path.join(collect(src, '--keep' in sys.argv), name)
    if not os.path.exists(path):
        raise SystemExit('sched.py: dump.sh collected no %s for %s' % (name, src))
    sec = section(open(path, encoding='utf-8', errors='replace').read(), func)
    decisions, rtl = split_rtl(sec)

    if '--insn' in sys.argv:
        n = int(sys.argv[sys.argv.index('--insn') + 1])
        print(one_insn(rtl, n) or 'sched.py: no insn %d in this function' % n)
        return 0
    if '--rtl' in sys.argv:
        print(rtl, end='')
        return 0

    # Only the per-insn lines, and not the block tail's sentinel (gcc gives the
    # last insn a huge priority so the backward pass takes it first).
    pris = sorted({int(m.group(1)) for m in
                   re.finditer(r'^;; insn\[\s*\d+\]: priority =\s*(\d+)',
                               decisions, re.M)}
                  - {2147483647})
    pris = [x for x in pris if x < 1 << 20]
    print(decisions, end='')
    print(';; priorities present: %s%s'
          % (', '.join(str(x) for x in pris) or 'none',
             '  <-- ALL 1, so the priority key does nothing here and'
             '\n;;   schedule_select decides on potential_hazard instead'
             if pris == [1] else ''))
    print(';; %d line(s) of scheduled RTL follow in the dump'
          ' (--rtl prints them, --insn N prints one)' % rtl.count('\n'))
    return 0


if __name__ == '__main__':
    sys.exit(main())
