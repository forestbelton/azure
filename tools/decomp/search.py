#!/usr/bin/env python3
"""search.py BASE.c UNIT FUNC -- sweep local source transforms for a better match.

Takes a candidate whose *logic* is already right and mechanically explores the
source shapes that this compiler is sensitive to, scoring each with verify.py.
A verify cycle is about 0.7s, so a few hundred variants is a couple of minutes
of compute and no model tokens -- which is the point: enumerating variants and
reading their word counts was the single largest cost in this project's
decompilation sessions, and it is pure search over a scalar oracle.

It does NOT derive logic. Deciding what the function *does* from the listing is
the part a search cannot do, and a candidate that is structurally wrong will
not be rescued by any combination of these rewrites.

    python3 tools/decomp/search.py zz_foo.c unk_14934 func_80045CC4
    python3 tools/decomp/search.py zz_foo.c unk_14934 func_80045CC4 --sites
    python3 tools/decomp/search.py zz_foo.c unk_12DE0 func_8003F5E0 --g8

Greedy by default: try every single change against the current best, keep the
best improvement, repeat. `--exhaustive` enumerates all combinations instead,
which is only sane when `--sites` shows a handful.

Score is (|built words - target words|, differing words), lexicographic. Word
count is the reliable signal; the differing-word count is noisy, because one
missing word shifts everything after it -- func_80042BDC sat at "200 of 543
differing" from a single missing instruction.

The run ends by printing what each accepted change was worth, which is the
"load-bearing shapes, with the cost of reverting each" table a near-miss header
wants. Keep it: it is the measurement, and it is what stops the next attempt
re-deriving the same thing.
"""
import os
import re
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.environ.get("AZURE_ROOT", os.path.dirname(os.path.dirname(HERE)))
SCRATCH = os.environ.get("AZURE_SCRATCH", "/tmp/azure-decomp")

WIDTHS = ["s32", "u32", "s16", "u16", "s8", "u8"]

# Every transform is restricted to the target function's BODY. Without that,
# a near-miss file's header comment and its local struct typedefs are fair
# game: the first real run of this tool "fixed" func_80045CC4's missing word by
# narrowing a u16 STRUCT FIELD to u8, which shifts every later field offset --
# a semantic change dressed as a shape change, and it tripled the differing
# count while the word count looked better. It also cut the candidate count on
# that function from 435 to the handful that are actually local variables.
SPAN = (0, 0)


def body_span(text, func):
    """(start, end) of the function's body, or the whole file if not found."""
    m = re.search(r"(?m)^[A-Za-z_][\w \*]*\b%s\s*\([^;{]*\)\s*\{"
                  % re.escape(func), text)
    if not m:
        return 0, len(text)
    i, depth = m.end(), 1
    while i < len(text) and depth:
        if text[i] == "{":
            depth += 1
        elif text[i] == "}":
            depth -= 1
        i += 1
    return m.end(), i - 1


def scan(pattern, text):
    """finditer, restricted to the current function body."""
    lo, hi = SPAN
    for m in re.finditer(pattern, text):
        if lo <= m.start() and m.end() <= hi:
            yield m


# ---------------------------------------------------------------- transforms
#
# Each transform is a generator of (label, new_text) pairs over one base text.
# They are deliberately textual and conservative: a rewrite that cannot be
# applied safely by regex is better left to the model than guessed at here.

def t_decl_width(text):
    """Swap a local's declared width. Worth 8 words in func_8003D0F0 (the s16
    accumulator that blocks combine's byte-to-signed refold) and 2 in
    func_80042BDC (a u8 local that keeps `addiu 0xFF`)."""
    for m in scan(r"(?m)^([ \t]+)(%s)(\s+)(\w+)(\s*(?:;|=))" % "|".join(WIDTHS), text):
        cur = m.group(2)
        for w in WIDTHS:
            if w == cur:
                continue
            new = text[: m.start(2)] + w + text[m.end(2) :]
            yield ("decl %s: %s -> %s" % (m.group(4), cur, w), new)


def t_drop_cast16(text):
    """Remove an (s16)/(s8) cast. `lhu` where the target has `lh` means the
    cast belongs; the reverse means it does not."""
    for m in scan(r"\((s16|s8)\)\s*", text):
        yield ("drop (%s) at %d" % (m.group(1), m.start()),
               text[: m.start()] + text[m.end() :])


def t_add_cast16(text):
    """Add an (s16) cast to an array index. A u16 field or global read with an
    (s16) cast yields a bare `lh`; one differing word per site in
    func_80041B98 and three siblings."""
    for m in scan(r"\[([A-Za-z_]\w*(?:->\w+|\.\w+)*)\]", text):
        inner = m.group(1)
        yield ("cast index [%s] -> [(s16) %s]" % (inner, inner),
               text[: m.start()] + "[(s16) " + inner + "]" + text[m.end() :])


def t_split_binop(text):
    """Split `x = EXPR op CONST;` into `x = EXPR; x op= CONST;`.

    Two sets make the value opaque to combine's substitution across a block
    boundary, which keeps the chain one global allocno: worth 81 differing
    words in func_80041588's flag word and 22 in func_80041344's."""
    pat = re.compile(r"(?m)^([ \t]+)(\w+)\s*=\s*([^;=]+?)\s*(\||\&|\+|-|<<|>>)\s*([^;]+);")
    for m in scan(pat, text):
        ind, var, lhs, op, rhs = m.groups()
        if var in lhs:  # already in-place
            continue
        repl = "%s%s = %s;\n%s%s %s= %s;" % (ind, var, lhs, ind, var, op, rhs)
        yield ("split %s = ... %s ..." % (var, op),
               text[: m.start()] + repl + text[m.end() :])


def t_join_binop(text):
    """The reverse of t_split_binop, for when the target wants one insn."""
    pat = re.compile(r"(?m)^([ \t]+)(\w+)\s*=\s*([^;=]+);\n[ \t]+\2\s*(\||\&|\+|-|<<|>>)=\s*([^;]+);")
    for m in scan(pat, text):
        ind, var, lhs, op, rhs = m.groups()
        repl = "%s%s = %s %s %s;" % (ind, var, lhs, op, rhs)
        yield ("join %s %s=" % (var, op),
               text[: m.start()] + repl + text[m.end() :])


def t_cmp_swap(text):
    """`a < b` and `b > a` emit the same `slt` but evaluate their operands in
    source order, so they differ in which load comes first: 2 words in
    func_80042BDC and about 2 at each of two sites in func_80045CC4."""
    flip = {"<": ">", ">": "<", "<=": ">=", ">=": "<="}
    pat = re.compile(r"\(([^()]+?)\s*(<=|>=|<|>)\s*([^()]+?)\)")
    for m in scan(pat, text):
        a, op, b = m.group(1), m.group(2), m.group(3)
        if "=" in a or "=" in b:
            continue
        repl = "(%s %s %s)" % (b, flip[op], a)
        yield ("swap %s %s %s" % (a.strip(), op, b.strip()),
               text[: m.start()] + repl + text[m.end() :])


def t_mask1(text):
    """`if (x != 0)` versus `if ((x & 1) != 0)`. On a 0/1 value the mask plus a
    `u8` declaration keeps a one-insn materialisation that the bare test does
    not emit at all -- the word func_80042BDC was missing."""
    for m in scan(r"\((\w+)\s*!=\s*0\)", text):
        v = m.group(1)
        yield ("mask %s != 0 -> (%s & 1) != 0" % (v, v),
               text[: m.start()] + "((%s & 1) != 0)" % v + text[m.end() :])
    for m in scan(r"\(\((\w+)\s*&\s*1\)\s*!=\s*0\)", text):
        v = m.group(1)
        yield ("unmask (%s & 1) != 0 -> %s != 0" % (v, v),
               text[: m.start()] + "(%s != 0)" % v + text[m.end() :])


TRANSFORMS = [
    t_decl_width,
    t_drop_cast16,
    t_add_cast16,
    t_split_binop,
    t_join_binop,
    t_cmp_swap,
    t_mask1,
]


# -------------------------------------------------------------------- oracle

BUILT = re.compile(r"built (\d+) words, target (\d+) words")
DIFF = re.compile(r"RESULT: (?:(\d+) DIFF|MATCH)")


def score(text, unit, func, g8, cache):
    """(|word delta|, differing words) for one candidate; None if it won't build."""
    if text in cache:
        return cache[text]
    fd, path = tempfile.mkstemp(suffix=".c", prefix="zzsearch_", dir=PROJ)
    try:
        with os.fdopen(fd, "w") as fh:
            fh.write(text)
        cmd = ["python3", os.path.join(HERE, "verify.py"),
               os.path.basename(path), unit, func]
        if g8:
            cmd.append("--g8")
        r = subprocess.run(cmd, cwd=PROJ, capture_output=True, text=True)
        out = r.stdout
        mb, md = BUILT.search(out), DIFF.search(out)
        if not mb or not md:
            res = None
        else:
            built, target = int(mb.group(1)), int(mb.group(2))
            res = (abs(built - target), int(md.group(1) or 0), built, target)
    finally:
        os.unlink(path)
    cache[text] = res
    return res


def better(cand, cur):
    """Is `cand` a strict improvement on `cur`?

    Fewer differing words with the word count no further off, or a closer word
    count with the differing count no worse. Buying a word at the cost of a
    much larger differing count is almost always a semantic change rather than
    a shape change, so it is reported separately for a human to judge instead
    of being chained into.
    """
    return ((cand[0] <= cur[0] and cand[1] < cur[1])
            or (cand[0] < cur[0] and cand[1] <= cur[1]))


def fmt(s):
    if s is None:
        return "does not build"
    if s[0] == 0 and s[1] == 0:
        return "MATCH (%d words)" % s[2]
    return "%d words (%+d), %d diff" % (s[2], s[2] - s[3], s[1])


def main():
    # a run is minutes long, so stream progress rather than buffering it
    try:
        sys.stdout.reconfigure(line_buffering=True)
    except AttributeError:
        pass
    flags = {a for a in sys.argv[1:] if a.startswith("--")}
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    if len(args) != 3:
        print((__doc__ or "").strip())
        return 2
    base_path, unit, func = args
    g8 = "--g8" in flags
    text = open(base_path).read()

    # --only=width,cast narrows the transform set. On a large function the full
    # set generates hundreds of sites -- func_80045CC4 gives 435 -- and one
    # greedy pass compiles every one of them. Narrow it to the families the
    # listing actually implicates: run annotate.py first and pick from there.
    active = TRANSFORMS
    for a in sys.argv[1:]:
        if a.startswith("--only="):
            want = [w.strip() for w in a.split("=", 1)[1].split(",") if w.strip()]
            active = [t for t in TRANSFORMS
                      if any(w in t.__name__ for w in want)]
            if not active:
                print("no transform matches --only=%s; available: %s"
                      % (",".join(want),
                         " ".join(t.__name__.replace("t_", "") for t in TRANSFORMS)))
                return 2

    global SPAN
    SPAN = body_span(text, func)
    sites = [(t.__name__, lbl) for t in active for lbl, _ in t(text)]
    if "--sites" in flags:
        print("%d candidate change(s) from %s:" % (len(sites), base_path))
        for name, lbl in sites:
            print("  %-16s %s" % (name.replace("t_", ""), lbl))
        return 0

    cache = {}
    print("%d candidate change(s) across %d transform famil(y|ies)"
          % (len(sites), len(active)))
    best = score(text, unit, func, g8, cache)
    if best is None:
        print("the base does not build; fix that first")
        return 1
    print("base: %s" % fmt(best))
    if best[0] == 0 and best[1] == 0:
        print("already a match -- nothing to search for")
        return 0

    budget = 400
    for a in sys.argv[1:]:
        if a.startswith("--max-compiles="):
            budget = int(a.split("=", 1)[1])
    accepted = []
    flagged = []
    cur_text, cur = text, best
    passes = 0
    while passes < 8 and len(cache) < budget:
        passes += 1
        improved = None
        for t in active:
            for lbl, cand in t(cur_text):
                if len(cache) >= budget:
                    break
                s = score(cand, unit, func, g8, cache)
                if s is None:
                    continue
                if s[0] < cur[0] and not better(s, cur):
                    flagged.append((lbl, s))
                if better(s, cur) and (improved is None
                                       or better(s, improved[0])):
                    improved = (s, lbl, cand)
        if improved is None:
            break
        s, lbl, cand = improved
        print("  pass %d: %-44s %s" % (passes, lbl, fmt(s)))
        accepted.append((lbl, cur, s))
        cur_text, cur = cand, s
        if cur[0] == 0 and cur[1] == 0:
            break

    print()
    print("%d variant(s) compiled; best: %s" % (len(cache), fmt(cur)))
    if flagged:
        print()
        print("NOT accepted -- these close the word count but cost differing")
        print("words, so judge them by hand (a struct field narrowed to fix a")
        print("word is a layout change, not a shape change):")
        for lbl, s in flagged[:8]:
            print("  %-46s %s" % (lbl, fmt(s)))
    if not accepted:
        print("no single change improved on the base. Either the shape is")
        print("already right and what is left is allocation or ordering (read")
        print("docs/matching-levers.md sections 3 and 8), or the logic is wrong")
        print("and no local rewrite will fix it.")
        return 0

    out = os.path.join(SCRATCH, "search_best.c")
    os.makedirs(SCRATCH, exist_ok=True)
    with open(out, "w") as fh:
        fh.write(cur_text)
    print("best variant written to %s" % out)
    print()
    print("load-bearing changes, with the cost of reverting each:")
    for lbl, before, after in accepted:
        print("  %-46s %s -> %s" % (lbl, fmt(before), fmt(after)))
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except BrokenPipeError:
        # piped into head/less: exit quietly rather than dumping a traceback
        os.dup2(os.open(os.devnull, os.O_WRONLY), sys.stdout.fileno())
        sys.exit(0)
