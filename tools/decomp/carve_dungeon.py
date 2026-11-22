"""carve.py BOUNDARY_FUNC... -- split csrc/ovl_dungeon.c at the named functions.

Each boundary starts a new translation unit named ovl_dungeon_<FUNC-suffix>.
Prototypes for callees defined in an EARLIER unit are synthesised from those
units' own definition lines, because a callee defined above its caller is a
DECLARED function and across a unit boundary it would silently become an
implicit `int f()'. Declarations stay shared through ovl_dungeon_decls.h.
"""
import re, sys, os

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import parkresidue as pr

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
        # A PARKED FUNCTION'S CALLEES ARE INVISIBLE IN `ch', AND THAT COST
        # func_800ADE74 FOUR ROWS ON 2026-11-22.
        #
        # The callee set above is read out of the unit's own C TEXT. A parked
        # function contributes nothing to that text but its `INCLUDE_ASM(...)'
        # line, so none of the functions its PARK BODY calls is named here and
        # no prototype is synthesised for them. The park then compiles against
        # implicit `int f()' declarations that the single pre-carve unit had
        # supplied for free, and its residue changes.
        #
        # The third carve moved func_800ADE74 into ovl_dungeon_800A4B88 and it
        # went 30 rows -> 34. Fourteen of its callees were newly undeclared;
        # bisecting them one at a time showed THIRTEEN MOVE NOTHING and the
        # whole four rows are one prototype:
        #
        #     s16 func_800A45D8(u16 x, u16 y, s32 c);
        #
        # and specifically the `u16' on the first two parameters -- the call
        # passes `sp18 << 6' from s16 stack locals, so u16 is a narrowing the
        # target performs. Every spelling with u16/u16 and a 32-bit-or-wider
        # return measures 30; widening or signing them measures 34, exactly
        # like no prototype at all.
        #
        # So union in the callees of every park this unit stubs, read from the
        # park's LIVE BODY (body_of, which skips `#if 0' alternatives) and not
        # from its header, since a header names functions in prose. This
        # cannot disturb a landed function: if one in this unit called the
        # same callee, the name would already be in `ch'.
        for pf in re.findall(r'INCLUDE_ASM\("[^"]*",\s*(func_[0-9A-Fa-f]{8})\)', ch):
            pk = os.path.join('docs/dungeon-parks', pf + '.c')
            if not os.path.exists(pk):
                continue
            try:
                b = pr.body_of(pk, pf)
            except Exception:
                b = ''
            if b:
                named |= set(re.findall(r'\b(func_[0-9A-Fa-f]{8})\b', b))
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
