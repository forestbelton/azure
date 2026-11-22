"""carve.py BOUNDARY_FUNC... -- split csrc/ovl_dungeon.c at the named functions.

Each boundary starts a new translation unit named ovl_dungeon_<FUNC-suffix>.
Prototypes for callees defined in an EARLIER unit are synthesised from those
units' own definition lines, because a callee defined above its caller is a
DECLARED function and across a unit boundary it would silently become an
implicit `int f()'. Declarations stay shared through ovl_dungeon_decls.h.
"""
import re, sys, os

SRC = 'csrc/ovl_dungeon.c'
HDR = 'csrc/ovl_dungeon_decls.h'
bounds = sys.argv[1:]
s = open(SRC).read()
hdr = open(HDR).read()
pre_end = s.index('\n\n', s.index('#include "ovl_dungeon_decls.h"')) + 2
preamble, rest = s[:pre_end], s[pre_end:]

cuts = [0]
for f in bounds:
    m = re.search(r'(?m)^(INCLUDE_ASM\("asm/nonmatchings/ovl_dungeon", ' + f + r'\);|'
                  r'[A-Za-z_][^\n;]*\b' + f + r'\s*\([^;{]*\)\s*\n\{)', rest)
    assert m, "no top-level %s" % f
    cuts.append(m.start())
cuts.append(len(rest))
assert cuts == sorted(cuts), "boundaries are out of file order: %s" % cuts
chunks = [rest[cuts[i]:cuts[i+1]] for i in range(len(cuts)-1)]
names = ['ovl_dungeon'] + ['ovl_dungeon_' + f.split('_')[1] for f in bounds]

def defs_in(text):
    out = {}
    for m in re.finditer(r'(?m)^[A-Za-z_][^\n;]*?\b(func_[0-9A-Fa-f]{8})\s*\(', text):
        k = m.end() - 1; d = 0
        while True:
            if text[k] == '(': d += 1
            elif text[k] == ')':
                d -= 1
                if d == 0: break
            k += 1
        if text[k+1:].lstrip().startswith('{'):
            out.setdefault(m.group(1), " ".join(text[m.start():k+1].split()))
    return out

earlier = {}
for i, (nm, ch) in enumerate(zip(names, chunks)):
    # REWRITE EVERY INCLUDE_* MACRO, NOT JUST INCLUDE_ASM. The first version
    # matched the literal `INCLUDE_ASM("asm/nonmatchings/ovl_dungeon",', which
    # left seven INCLUDE_RODATA lines pointing at the old unit after the third
    # carve on 2026-11-22. splat MOVES each .s into its new unit's directory,
    # so the stale path is simply gone and the build dies in the assembler
    # with `can't open asm/nonmatchings/ovl_dungeon/D_80089000.s'. Matching the
    # path itself catches INCLUDE_ASM, INCLUDE_RODATA and anything else.
    body = ch.replace('"asm/nonmatchings/ovl_dungeon"',
                      '"asm/nonmatchings/%s"' % nm)
    protos = []
    if i:
        named = set(re.findall(r'\b(func_[0-9A-Fa-f]{8})\b', ch))
        for f in sorted(named & set(earlier)):
            if re.search(r'(?m)^[A-Za-z_][^\n;]*\b' + f + r'\s*\([^;]*;', hdr):
                continue
            protos.append(earlier[f] + ';')
    path = 'csrc/%s.c' % nm
    if i == 0:
        out = preamble + body
    else:
        out = ('#include "common.h"\n#include "ovl_dungeon_decls.h"\n\n'
               '/* dungeon.bin\'s overlay, translation unit %d of %d: %s to the next\n'
               ' * boundary.  THE SPLIT IS FORCED BY JUMP-TABLE ALIGNMENT, not by size --\n'
               ' * see splat/dungeon.bin.yaml for the offsets and why each one is where it\n'
               ' * is.  The prototypes below are what this unit used to get for free by\n'
               ' * being in the same file as the definitions; they are generated from those\n'
               ' * definition lines, so they say exactly what the single-unit compile said.\n'
               ' */\n\n' % (i + 1, len(names), bounds[i-1])
               + ('\n'.join(protos) + '\n\n' if protos else '')
               + body)
    open(path, 'w').write(out.rstrip('\n') + '\n')
    print("%-26s %6d lines, %3d defs, %d protos" % (path, out.count('\n'), len(defs_in(ch)), len(protos)))
    earlier.update(defs_in(ch))
