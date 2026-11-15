"""Write a permuter-safe copy of a unit source.

decomp-permuter parses base.c with pycparser, which CANNOT parse an `__asm__`
operand containing a binary operator: `gte_stsxy(q + 0x20)` expands to
`:: "r"( q + 0x20 )` and the parse dies on the `+`.  ONE such call anywhere in
the unit breaks the import of EVERY function in it, and the only symptom is
`base=?` in the permrun log.

The permuter scores one function, and other functions' bodies do not affect
its codegen, so hoisting these operands into locals in the COPY ONLY is free.
Never do this to csrc -- the landings were measured against the inline form.

Usage: permsafe.py <src> <dst>
"""
import io, re, sys

src, dst = sys.argv[1], sys.argv[2]
s = io.open(src, encoding="utf-8").read()

# One single-argument gte_* call per line; greedy .* takes the last `)`.
pat = re.compile(r"^(?P<ind>[ \t]*)gte_(?P<name>\w+)\((?P<arg>.*)\);[ \t]*$", re.M)
n = 0


def fix(m):
    global n
    arg = m.group("arg")
    if "," in arg:
        return m.group(0)                      # multi-argument: nothing to hoist
    # `sp->unk_70[0]` carries a `-` that belongs to `->`, so strip arrows first.
    if not any(ch in arg.replace("->", "") for ch in "+-*"):
        return m.group(0)
    n += 1
    ind = m.group("ind")
    return ("%s{\n%s    u8 *permsafe_%d = (u8 *)(%s);\n%s    gte_%s(permsafe_%d);\n%s}"
            % (ind, ind, n, arg.strip(), ind, m.group("name"), n, ind))


s = pat.sub(fix, s)
io.open(dst, "w", encoding="utf-8").write(s)
print("permsafe: %d gte operand(s) hoisted" % n)
