#!/usr/bin/env python3
"""unname.py [FUNC ...] -- delete each local from a park and re-score it.

WHETHER A VALUE HAS A NAME IS A COMPILER SWITCH, not a matter of taste. A
declared local's pseudo carries `REG_USERVAR_P`; one that expand or cse
invented does not (the dumps print it as `/v`). On this target that flag
decides two things, both in loop.c:

  * whether a giv pays `copy_cost` (loop.c:3797, and again for an absorbed giv
    at loop.c:5521) -- i.e. whether an address is strength-reduced at all;
  * whether an invariant can be hoisted to the preheader (loop.c:698).

Removing a local also takes a materialisation out of its block's ready list,
which moves sched1 ties and, downstream of them, register assignments.

So for every local this tries the source with that local DELETED and its value
written out at each use, and reports the ones that score better. Two transforms,
both applied one local at a time:

    u8 *q = D_80084918;  ... q + 4 ...   ->   ... (D_80084918) + 4 ...
    u8 *e;  e = b + n * 0x78;  ... e ... ->   ... (b + n * 0x78) ...

Locals that are reassigned, or whose value depends on something that changes
between the assignment and a use, are skipped -- the first because the
substitution will not compile, the second because `sets != 1` rejects it.

Scoring is verify.py's, lexicographic on (|built - target| words, differing
words), the same as perm.py and search.py.

THREE WARNINGS, all earned on 2026-10-28, the day this found its first hits.
The first is the one to take seriously, because the guards below were each
written AFTER the sweep proposed a wrong program with good numbers:

  * A HIT CAN BE A DIFFERENT PROGRAM. The guards reject the two classes that
    actually came up -- an operand written between the assignment and the use
    (func_8003A904's `(*cp)++`), and a value that is a CALL or a memory read
    duplicated across several uses (func_8004A330's `func_8004B404(0)`, which
    scored 79 words / 69 DIFF against 74 / 74 by calling it twice). They are
    syntactic and conservative, not a proof. Read what a hit actually does.
  * A BETTER NUMBER CAN BE THE WRONG SOURCE. On func_80056D44 this reports
    27/28 words and 25 DIFF against the park's 26/27 -- better on both counts
    -- but with no local gcc walks the base as a CURSOR (`addiu $v1, $v1, 0x4`
    then `sw $v0, -0x4($v1)`) where the listing holds one base throughout. Read
    the diff before adopting a hit.
  * GIVE IT ITS OWN WINE PREFIX if anything else is compiling. Concurrent
    `ccpsx` runs share one prefix and the loser exits 0 having written nothing,
    so a sweep run beside hand measurements corrupts both:

        cp -a ~/.cache/azure-psyq /tmp/azure-wine-sweep
        AZURE_WINEPREFIX=/tmp/azure-wine-sweep AZURE_SCRATCH=/tmp/azure-decomp-sweep \\
            python3 tools/decomp/unname.py

With no arguments it sweeps every park in docs/near-misses. Name one or more
functions to sweep only those.
"""
import glob, os, re, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.environ.get("AZURE_ROOT", os.path.dirname(os.path.dirname(HERE)))
SP = os.environ.get("AZURE_SCRATCH", "/tmp/azure-decomp")
os.makedirs(SP, exist_ok=True)

# `TYPE *name = init;` -- a declaration carrying its value
DECL_INIT = re.compile(r"^(\s+)((?:const\s+)?[A-Za-z_]\w*\s*\**\s*)(\w+)\s*=\s*([^;]+);\s*$")
# `TYPE *name;` -- a declaration without one
DECL_BARE = re.compile(r"^\s+(?:const\s+)?[A-Za-z_]\w*\s*\**\s*(\w+)\s*;\s*$")
# `name = rhs;` -- a plain assignment, not a comparison or a compound op
ASSIGN = re.compile(r"^(\s+)(\w+)\s*=\s*([^;=][^;]*);\s*$")

KEYWORDS = {"return", "else", "break", "case", "goto", "continue", "do", "while", "if"}


def score(path, unit, func):
    """verify.py's (words off, differing words), or None if it did not build."""
    r = subprocess.run([sys.executable, os.path.join(HERE, "verify.py"), path, unit, func],
                       capture_output=True, text=True, cwd=PROJ)
    m = re.search(r"built (\d+) words, target (\d+) words", r.stdout)
    res = re.search(r"RESULT: (MATCH|(\d+) DIFF)", r.stdout)
    if not m or not res:
        return None
    nd = 0 if res.group(1) == "MATCH" else int(res.group(2))
    return (abs(int(m.group(1)) - int(m.group(2))), nd, int(m.group(1)))


def n_sets(lines, name):
    """How many statements assign to `name` (plain, compound, ++ or --)."""
    pat_lhs = re.compile(r"^\s*%s\s*(=[^=]|\+\+|--|\+=|-=|\*=|/=|\|=|&=|\^=|<<=|>>=)"
                         % re.escape(name))
    pat_inc = re.compile(r"\b%s\s*(\+\+|--)|(\+\+|--)\s*%s\b" % (re.escape(name),
                                                                 re.escape(name)))
    return sum(1 for l in lines if pat_lhs.match(l) or pat_inc.search(l))


def touched(lines, name):
    """True if `name` is written ANYWHERE in `lines`, directly or through it.

    Moving a value to its use is only sound while every operand of that value
    still holds what it held at the assignment. This is the guard for that, and
    it is deliberately blunt: `name`, `*name`, `(*name)++`, `name[i]`,
    `name->f` on the left of an assignment or carrying an increment all count.

    It is not paranoia. Without it the sweep reports func_8003A904's
    `m = *(u32 *) (s + **cp * 4 + 0x48) * 4;` as 44 words / 42 DIFF against the
    park's 46 / 44 -- a real improvement on both axes, and WRONG, because
    `(*cp)++` runs between that line and `m`'s only use, so the inlined form
    reads the incremented pointer. A sweep that proposes a semantic change is
    worse than one that finds nothing.
    """
    pat = re.compile(
        r"^\s*\(*\**\s*%s\b[^=;]*?(=[^=]|\+\+|--|\+=|-=|\*=|/=|\|=|&=|\^=|<<=|>>=)"
        % re.escape(name))
    inc = re.compile(r"(\+\+|--)\s*\(*\**\s*%s\b|\b%s\b\s*\)*\s*(\+\+|--)"
                     % (re.escape(name), re.escape(name)))
    return any(pat.match(l) or inc.search(l) for l in lines)


def variants(text):
    """Yield (label, source) with one local deleted and inlined."""
    lines = text.split("\n")
    start = next((i for i, l in enumerate(lines) if l.startswith("#include")), 0)
    bare = {}
    for i in range(start, len(lines)):
        d = DECL_BARE.match(lines[i])
        if d and d.group(1) not in KEYWORDS:
            bare[d.group(1)] = i

    for i in range(start, len(lines)):
        d = DECL_INIT.match(lines[i])
        if d:
            ty, name, init = d.group(2), d.group(3), d.group(4)
            if ty.strip() in KEYWORDS or name in KEYWORDS:
                continue
            if not ty.rstrip().endswith("*") and "*" not in ty and not re.search(r"[+*\[]", init):
                continue            # a plain scalar copy is not worth a compile
            drop, want_sets = [i], 0      # the declaration IS the set
        else:
            a = ASSIGN.match(lines[i])
            if not a:
                continue
            name, init = a.group(2), a.group(3)
            if name not in bare or name in KEYWORDS:
                continue
            drop, want_sets = [i, bare[name]], 1

        if n_sets(lines[start:], name) != want_sets:
            continue
        uses = sum(len(re.findall(r"\b%s\b" % re.escape(name), l)) for l in lines[i + 1:])
        if not 1 <= uses <= 14:
            continue
        # every operand of the value must be unchanged from here to the last use
        operands = set(re.findall(r"\b([A-Za-z_]\w*)\b", init)) - KEYWORDS - {name}
        if any(touched(lines[i + 1:], o) for o in operands):
            continue
        # A CALL is not a value you can move or copy: inlining a local that
        # holds one either duplicates its side effects (more than one use) or
        # reorders them against another call. func_8004A330's
        # `r = func_8004B404(0);` is used twice, and the sweep scored the
        # duplicated form 79 words / 69 DIFF against 74 / 74 -- better, and two
        # calls where the program has one.
        has_call = re.search(r"\b\w+\s*\(", init)
        calls_between = [l for l in lines[i + 1:] if re.search(r"\b\w+\s*\(", l)]
        if has_call and (uses > 1 or calls_between):
            continue
        # A local holding a LOADED value, used more than once, becomes several
        # loads when it is inlined -- which differs if anything could have
        # written that memory in between. A call is the cheap proxy for that.
        # `&x[i]` is exempt: it is an address, not a read.
        body = lines[i + 1:]
        last = max(j for j, l in enumerate(body)
                   if re.search(r"\b%s\b" % re.escape(name), l))
        reads_mem = (not init.strip().startswith("&")
                     and re.search(r"\*\s*\(|->|\w\s*\[", init))
        if uses > 1 and reads_mem and any(re.search(r"\bfunc_[0-9A-Fa-f]{6,8}\s*\(|"
                                                    r"\b[A-Z]\w*\s*\(", l)
                                          for l in body[:last + 1]):
            continue
        rep = "(" + init.strip() + ")"
        out = []
        for j, l in enumerate(lines):
            if j in drop:
                continue
            out.append(re.sub(r"\b%s\b" % re.escape(name), rep, l) if j > start else l)
        yield name, "\n".join(out)


want = set(sys.argv[1:])
parks = sorted(glob.glob(os.path.join(PROJ, "docs/near-misses/*.c")))
hits = 0
for f in parks:
    func = os.path.basename(f)[:-2]
    if want and func not in want:
        continue
    text = open(f).read()
    m = re.search(r"\((\w+)\)", text.split("\n")[0])
    if not m:
        continue
    unit = m.group(1)
    if not glob.glob(os.path.join(PROJ, "asm/nonmatchings/%s/%s.s" % (unit, func))):
        print("%-16s SKIP -- no listing under %s" % (func, unit))
        continue
    base = score(f, unit, func)
    if base is None:
        print("%-16s SKIP -- does not build" % func)
        continue
    if base[1] == 0:
        continue
    scratch = os.path.join(SP, "unname_%s.c" % func)
    for name, src in variants(text):
        open(scratch, "w").write(src)
        r = score(scratch, unit, func)
        if r and r[:2] < base[:2]:
            hits += 1
            print("%-16s drop `%s`: %d words / %d DIFF  ->  %d words / %d DIFF"
                  % (func, name, base[2], base[1], r[2], r[1]), flush=True)
print("\n%d improvement(s). Read the diff before adopting one -- see this file's header."
      % hits)
