#!/usr/bin/env python3
"""relocdump.py FILE.c [FUNC...] -- disassemble a candidate and show its RELOCATIONS.

    python3 tools/decomp/relocdump.py scratch/pa_unit.c func_8008BC58
    python3 tools/decomp/relocdump.py scratch/pa_unit.c            # every function

Why this exists. `ovlverify.py` MASKS every relocated word, so a clean masked
run says nothing about them, and one class -- a `%hi`/`%lo` pair on a FUNCTION
symbol, which has no `jal` to decode -- is invisible to both standard checks
and has already broken a build in this sweep. Six separate agents hand-rolled
this same dump before it was promoted; it is here so the seventh does not.

Two traps baked in, each of which cost someone a run:

* **`-G0` is mandatory.** The overlays build at `-G0`, and `asmlib.compile_asm`
  defaults to `-G8`. A `-G8` dump is a different compilation and proves
  nothing about the bytes the build produces. A helper copied out of
  `scratch/` was once found silently dumping `-G8`.

* **`nm -n` emits `gcc2_compiled.` and `__gnu_compiled_c` as `t` symbols at
  address 0.** Left in, the boundary loop below gives the first real function
  zero length and the tool prints NOTHING AT ALL, with no error -- which is
  how one agent lost a measurement before rewriting it. They are filtered.

How to use the output. Read every `%hi`/`%lo` off the target's OWN splat
listing, then check the symbols and counts here against it. The arithmetic
that closes the loop is:

    ovlverify --raw differing  ==  relocation words + intra-function `j` words

Both sides are countable: the right-hand side from the listing with
`grep -oE '%(hi|lo)\\((func_|D_)[0-9A-Fa-f]+\\)'` plus `grep -c '\\bjal\\b'`
plus the `j .L` count. If the arithmetic does not close, a REAL difference is
hiding behind the mask. Worked examples that did close: 28/7/0/26 against
18+5+5, 6+0+1, 0+0+0 and 19+5+2; and 38 against 9+29+0.

A function with no relocations at all has `--raw` equal to its masked run,
which is the strongest statement available about it.
"""
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import asmlib  # noqa: E402

# nm -n reports these at address 0; left in, the boundary loop below gives the
# first real function zero length and the whole dump comes out empty.
NM_NOISE = ('gcc2_compiled.', '__gnu_compiled_c')


def dump(src, want=()):
    # -G0, not asmlib's default -G8: see the module docstring.
    asm = asmlib.compile_asm(src, cflags=('-G0',))
    o = asmlib.assemble(asm, 'relocdump', 0)

    d = subprocess.run(['mips64-linux-gnu-objdump', '-d', '-z', '-r',
                        '-j', '.text', o],
                       capture_output=True, text=True).stdout
    nm = subprocess.run(['mips64-linux-gnu-nm', '-n', o],
                        capture_output=True, text=True).stdout

    syms = []
    for ln in nm.splitlines():
        m = re.match(r'([0-9a-f]+)\s+[tT]\s+(\S+)', ln)
        if m and m.group(2) not in NM_NOISE:
            syms.append((int(m.group(1), 16), m.group(2)))
    if not syms:
        print('relocdump: no text symbols -- did the compile fail?')
        return 1

    rows, cur = {}, None
    for ln in d.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', ln)
        if m:
            cur = int(m.group(1), 16)
            rows.setdefault(cur, [m.group(2), m.group(3).strip(), []])
            continue
        m = re.search(r'([0-9a-f]+):\s+(R_MIPS_\S+)\s+(\S+)', ln)
        if m and cur is not None:
            rows.setdefault(int(m.group(1), 16), ['', '', []])[2].append(
                (m.group(2), m.group(3)))

    shown = 0
    for i, (a, n) in enumerate(syms):
        if want and n not in want:
            continue
        shown += 1
        end = syms[i + 1][0] if i + 1 < len(syms) else max(rows) + 4
        print('=== %s  (base 0x%x, %d words) ===' % (n, a, (end - a) // 4))
        nrel, byfunc = 0, 0
        for addr in sorted(rows):
            if not (a <= addr < end):
                continue
            w, txt, rl = rows[addr]
            print('  %04x %s %s' % (addr - a, w, txt))
            for r in rl:
                nrel += 1
                if r[1].startswith('func_'):
                    byfunc += 1
                print('        RELOC %-18s %s' % r)
        print('  -> %d relocation word(s)%s' % (
            nrel,
            ', %d ON A FUNCTION SYMBOL -- the masked, build-breaking class'
            % byfunc if byfunc else ''))
    if want and not shown:
        print('relocdump: none of %s is in this object; it has: %s'
              % (', '.join(want), ', '.join(n for _, n in syms[:12])))
        return 1
    return 0


def main():
    if len(sys.argv) < 2:
        print((__doc__ or '').strip().split('\n\n')[0])
        print('\n    python3 tools/decomp/relocdump.py FILE.c [FUNC...]')
        return 1
    return dump(sys.argv[1], tuple(sys.argv[2:]))


if __name__ == '__main__':
    sys.exit(main())
