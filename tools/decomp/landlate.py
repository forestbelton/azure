"""Land candidates whose only collision is a prototype BELOW their own stub.

sweep.py's land refuses any unit where a name being defined also has an inline
prototype, because that collision broke the build three times. When the
prototype sits after the stub it is a legal prototype-after-definition, so this
lands those by hand and leaves the rest to sweep.py.
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
    assert m.start() > t.index(stub), '%s: prototype is ABOVE the stub' % name
    t = t.replace(stub, body)
    io.open(p, 'w', encoding='utf-8').write(t)
    print('landed %s by hand (prototype below it: %s)' % (name, m.group(0).strip()))
io.open(path.replace('.py', '_rest.py'), 'w').write('CANDS = %r\n' % (rest,))
print('wrote %s with %d left for sweep.py' % (path.replace('.py', '_rest.py'), len(rest)))
