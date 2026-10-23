"""Land candidates that already have an inline prototype, and say so.

sweep.py's land refuses any unit where a name being defined also has an inline
prototype, because a prototype that DISAGREES with the definition broke the
build three times. The sweep creates agreeing ones for itself constantly: every
landed function that calls a still-stubbed one declares it inline, and both
orders are legal C -- a prototype above its definition is ordinary, and one
below it is a prototype-after-definition.

So this lands them and prints each prototype with its position. It cannot
decide whether the types agree, and that is the whole hazard: `void
func_8004B1A4(s32)` against a `u8 *` definition is a compile error. `make
check` is the check -- run it before committing, as always.
"""
import io, re, runpy, sys

path = sys.argv[1]
cands = runpy.run_path(path)['CANDS']
rest = []
for name, unit, body in cands:
    p = 'csrc/%s.c' % unit
    t = io.open(p, encoding='utf-8').read()
    stub = 'INCLUDE_ASM("asm/nonmatchings/%s", %s);' % (unit, name)
    m = re.search(r'^[A-Za-z_][\w \*]*?\b%s\s*\([^;{]*\);\s*$' % name, t, re.M)
    if not m:
        rest.append((name, unit, body))
        continue
    assert t.count(stub) == 1, name
    where = 'below' if m.start() > t.index(stub) else 'above'
    t = t.replace(stub, body)
    io.open(p, 'w', encoding='utf-8').write(t)
    print('landed %s by hand (prototype %s it: %s)' % (name, where, m.group(0).strip()))
    print('  CHECK the types agree -- this script cannot')
io.open(path.replace('.py', '_rest.py'), 'w').write('CANDS = %r\n' % (rest,))
print('wrote %s with %d left for sweep.py' % (path.replace('.py', '_rest.py'), len(rest)))
