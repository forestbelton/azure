#!/usr/bin/env python3
"""verify.py zz_file.c UNIT FUNC -- compile, assemble via maspsx, compare .text words with the target listing.

Words carrying a relocation are compared on their upper 16 bits only, because
the linker supplies the rest. The low half is still checked against the addend
the splat listing implies and reported separately as `?`: a mismatch there is
usually real, but not always (a different symbol with a compensating addend
resolves to the same bytes), so it is a warning rather than a diff.

`make check` remains the only authority. In particular verify.py cannot see a
jump table landing at the wrong address.

Output is abbreviated by default: differing words, two words of context either
side, and runs of matching words collapsed to one line. Pass --full for the
whole side-by-side. The abbreviation matters -- the full form of a 437-word
function is about 10k tokens, and an agent iterating on it pays that again on
every subsequent turn.

When this says N DIFF and the shape is not obviously wrong, run `advise.py`
with the same arguments: it ALIGNS the two streams instead of comparing them
positionally -- so a one-word shift stops reading as N wrong words -- and
names the lever each surviving difference corresponds to.

The build pipeline lives in asmlib.py, so this tool, advise.py and nmcheck.py
cannot drift apart on compiler flags again.
"""
import sys

import os
import asmlib
from asmlib import CompileError

argv = [a for a in sys.argv[1:] if a != '--full']
full = len(argv) != len(sys.argv) - 1
src, unit, func = argv[0:3]
# --g8 models a unit built with gas left at -G8 rather than maspsx's forced
# -G0: gas then emits GPREL16 for an *undefined* symbol whose `.extern
# SYM,size` size is in (0, G]. NO UNIT IS BUILT THAT WAY ANY MORE -- the
# MASPSXFLAGS_G8/_G2 escape hatches were deleted on 2026-11-03 when the eleven
# carved gprel_* units were converted to file-scope definitions of their $gp
# objects. The flag is kept because the model is still the way to test what a
# nonzero -G would do to a declaration, and because the default below reads the
# Makefile, so it comes back by itself if a rule is ever added again.
# The carved units are built with a nonzero gas -G. Which value matters: gas
# goes $gp-relative only for a symbol whose `.extern SYM,size` gives a size in
# (0, G], and cc1psx emits that size from the declared type -- so -G2 keeps
# 2-byte objects $gp-relative while sending 4-byte ones to the absolute
# bare-symbol form. Verified directly against gas.
# Default it from the UNIT, via the Makefile's own per-unit rule: passing the
# wrong -G is silent, and a sweep over every park on 2026-10-27 measured
# func_8004437C at 107 words against its park's 100 purely because the sweep
# did not pass --g8 for a gprel_ unit.
gasG = asmlib.gas_g_for_unit(unit)
# cc1psx's OWN -G is a separate number from gas's, and it must come from the
# unit too. asmlib.CFLAGS hard-codes -G8 because the resident executable is
# -G8; the overlays carry a per-unit `CFLAGS := $(CFLAGS_G0)` override in the
# Makefile, and at -G8 a global access comes out as the assembler-expanded
# `lui $at`/`%lo` pair instead of cc1psx's own split address. Passing the
# wrong one is silent and total: it scored an already-matching
# ovl_main_80020000 park at 78 DIFF on 2026-11-01, which read as the function
# being wrong rather than the gate. compile_asm appends these after CFLAGS and
# the later -G wins.
cc1G = asmlib.cc1_g_for_unit(unit)
for a in sys.argv[1:]:
    if a == '--g8':
        gasG = 8
    elif a.startswith('--gas-G='):
        gasG = int(a.split('=', 1)[1])
    elif a == '--g0':
        gasG = 0
    elif a.startswith('--cc1-G='):
        cc1G = int(a.split('=', 1)[1])
miss = asmlib.missing_gp_defs(src, unit)
if miss:
    print('WARNING: this source references %s.' % ', '.join(miss))
    print('         %s DEFINES each of those and this source does not, so'
          % unit)
    print('         maspsx leaves the accesses absolute here and every')
    print('         reference costs a word MORE than it does in the unit.')
    print('         Add, above the function:')
    for line in miss.values():
        print('           %s' % line)
    print()
try:
    words, relocs = asmlib.build(src, gasG, cflags=('-G%d' % cc1G,))
except CompileError as e:
    print(e)
    sys.exit(2)
tgt = asmlib.target_words(unit, func)
listing_addend = asmlib.listing_addend

bad = 0; warn = 0
rows = []
for i in range(max(len(words), len(tgt))):
    if i < len(words) and i < len(tgt):
        a, w, txt = words[i]; t, ttxt = tgt[i]
        if a in relocs:
            sh = 26 if relocs[a] == '26' else 16
            ok = (w >> sh) == (t >> sh); mark = ' ' if ok else 'X'
            if ok and sh == 16:
                want = listing_addend(ttxt)
                if want is not None:
                    got = w & 0xFFFF
                    if got >= 0x8000 and want < 0:
                        got -= 0x10000
                    if got != want:
                        mark = '?'; warn += 1
        else:
            ok = w == t; mark = ' ' if ok else 'X'
        if not ok: bad += 1
        rows.append((mark, f'{mark} {w:08x} {txt:40s} | {t:08x} {ttxt}'))
    else:
        bad += 1
        rows.append(('X', 'X ' + (words[i][2] if i < len(words) else '<none>')
                     + ' | ' + (tgt[i][1] if i < len(tgt) else '<none>')))

print(f'built {len(words)} words, target {len(tgt)} words')
if full:
    for _, line in rows:
        print(line)
else:
    CTX = 2
    keep = set()
    for i, (mark, _) in enumerate(rows):
        if mark != ' ':
            for j in range(max(0, i - CTX), min(len(rows), i + CTX + 1)):
                keep.add(j)
    run = 0
    for i, (_, line) in enumerate(rows):
        if i in keep:
            if run:
                print(f'  ... {run} matching word(s) ...')
                run = 0
            print(line)
        else:
            run += 1
    if run:
        print(f'  ... {run} matching word(s) ...')
    if len(keep) < len(rows):
        print('  (--full for the whole side-by-side)')

tail = '' if not warn else f' (+{warn} relocated word(s) whose addend differs from the listing, marked ?; run make check)'
print('RESULT:', ('MATCH' if bad == 0 else f'{bad} DIFF') + tail)
