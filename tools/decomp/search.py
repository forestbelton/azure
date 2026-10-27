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
    python3 tools/decomp/search.py zz_foo.c unk_12DEC func_8003F5E0 --g8

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


def wrap(text, indent="  "):
    import textwrap
    return "\n".join(textwrap.wrap(text, 76, initial_indent=indent,
                                   subsequent_indent=indent))


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


def _match_brace(text, i):
    """Index just past the `}` closing the `{` at text[i]."""
    depth = 0
    while i < len(text):
        if text[i] == "{":
            depth += 1
        elif text[i] == "}":
            depth -= 1
            if depth == 0:
                return i + 1
        i += 1
    return -1


def t_dup_into_arms(text):
    """Move the statement after an if/else into BOTH arms.

    Cross-jumping runs in jump2, after both schedulers and after allocation, so
    the duplicate costs nothing in words -- and it buys the pseudos it mentions
    an extra reference at allocation time, which is how func_800423C0's
    register pair was flipped and how func_80045CC4's $s0/$s1 swap was closed.
    It is also the way to PREVENT a merge, when each arm's own last insn should
    stay in the arm.

    Only a single-line statement is moved, and only into an if that already has
    an else, so the rewrite stays a rewrite rather than a restructuring.
    """
    lo, hi = SPAN
    for m in re.finditer(r"(?m)^([ \t]+)\}\s*else\s*\{", text):
        if not (lo <= m.start() and m.end() <= hi):
            continue
        end = _match_brace(text, m.end() - 1)
        if end < 0 or end > hi:
            continue
        # the if's opening brace, walked back from this `} else {`
        head = text.rfind("if", lo, m.start())
        if head < 0:
            continue
        open_brace = text.find("{", head)
        if open_brace < 0 or open_brace > m.start():
            continue
        nxt = re.match(r"\n([ \t]+)([^\n{}]+;)\n", text[end:])
        if not nxt:
            continue
        stmt = nxt.group(2)
        ind = nxt.group(1)
        inner = ind + "    "
        body = (text[:m.start()].rstrip("\n") + "\n" + inner + stmt + "\n"
                + text[m.start():end - 1].rstrip("\n") + "\n" + inner + stmt
                + "\n" + ind + "}" + text[end + len(nxt.group(0)) - 1:])
        yield ("duplicate `%s` into both arms" % stmt[:40], body)


def _decl_run(text):
    """The run of plain declarations at the top of the function body.

    Returns [(line_start, line_end, name)] in source order. Stops at the first
    statement, so an initialised declaration (`u8 *p = q;`) ends the run -- it
    cannot be reordered freely anyway, because its initialiser may depend on
    an earlier one.
    """
    lo, hi = SPAN
    out = []
    i = lo
    while i < hi:
        j = text.find("\n", i)
        if j < 0 or j > hi:
            break
        line = text[i:j]
        if line.strip() == "":
            i = j + 1
            continue
        # the separator must be whitespace or a `*`, so a bare statement
        # (`foo;`) cannot pass as a declaration of `o` by type `fo`
        m = re.match(r"^[ \t]+[A-Za-z_]\w*(?:[ \t]+|[ \t]*\*+[ \t]*)(\w+)[ \t]*;[ \t]*$",
                     line)
        if not m:
            break
        out.append((i, j + 1, m.group(1)))
        i = j + 1
    return out


def t_decl_order(text):
    """Transpose two adjacent declarations.

    local-alloc assigns quantities in an order that depends on the pseudo
    numbering, and the pseudos are numbered in declaration order -- so this is
    a zero-cost knob on which of two equally-weighted locals wins the lower
    register. Greedy search composes the transpositions into any permutation.
    Documented as a tie-break in docs/compiler-version.md; it is the half of
    that tie-break a source rewrite can actually reach (the other half, block
    scope, is t_decl_per_arm below).
    """
    d = _decl_run(text)
    for k in range(len(d) - 1):
        a, b = d[k], d[k + 1]
        swapped = text[b[0]:b[1]] + text[a[0]:a[1]]
        yield ("declare %s before %s" % (b[2], a[2]),
               text[:a[0]] + swapped + text[b[1]:])


def _switch_arms(text):
    """[(arm_body_start, arm_body_end)] for one switch's arms in the body.

    Only the FIRST switch found is used, and only its own labels: an arm's
    nested switch is left alone. The returned span starts after the last label
    of a fallthrough run, so `case 1: case 2:` yields one arm.
    """
    lo, hi = SPAN
    m = re.search(r"(?m)^[ \t]*switch\b", text[lo:hi])
    if not m:
        return []
    ob = text.find("{", lo + m.end())
    if ob < 0:
        return []
    close = _match_brace(text, ob)
    if close < 0:
        return []
    labels = []
    depth = 0
    i = ob + 1
    while i < close:
        c = text[i]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
        elif depth == 0 and (text.startswith("case", i) or text.startswith("default", i)):
            if i == 0 or text[i - 1] in " \t\n":
                e = text.find(":", i)
                if e > 0:
                    labels.append((i, text.find("\n", e) + 1))
                    i = e
        i += 1
    arms = []
    for k, (_, body_start) in enumerate(labels):
        # fold a run of consecutive labels into the last one
        if k + 1 < len(labels) and text[body_start:labels[k + 1][0]].strip() == "":
            continue
        end = labels[k + 1][0] if k + 1 < len(labels) else close
        arms.append((body_start, end))
    return arms


def t_decl_per_arm(text):
    """Give each switch arm its own copy of a shared local, in a brace.

    A local reused across arms is ONE allocno whose references sum over all of
    them, which inflates its local-alloc priority and takes a register the
    target reuses -- the one-variable-per-loop rule in matching-levers. The fix
    is a declaration of the same name inside each arm that uses it, which is
    what took func_800517CC from 32 register-only words to MATCH (one brace per
    `case` was 28, one brace per block was the match).

    Only applied to a local used in two or more arms and nowhere outside them.
    """
    d = _decl_run(text)
    arms = _switch_arms(text)
    if not arms or not d:
        return
    lo, hi = SPAN
    for start, end, name in d:
        decl = text[start:end]
        word = re.compile(r"\b%s\b" % re.escape(name))
        uses = [m.start() for m in word.finditer(text) if lo <= m.start() < hi
                and not (start <= m.start() < end)]
        if not uses:
            continue
        inside = [a for a in arms if any(a[0] <= u < a[1] for u in uses)]
        if len(inside) < 2 or any(not any(a[0] <= u < a[1] for a in arms) for u in uses):
            continue
        ind = re.match(r"[ \t]*", decl).group(0)
        pieces = []
        last = 0
        for a0, a1 in inside:
            if not any(a0 <= u < a1 for u in uses):
                continue
            body = text[a0:a1].rstrip()
            trailing = text[a0:a1][len(body):]
            shifted = "\n".join(("    " + l) if l.strip() else l
                                 for l in body.split("\n"))
            pieces.append((a0, a1,
                           ind + "{\n" + ind + "    " + decl.strip() + "\n\n"
                           + shifted + "\n" + ind + "}" + trailing))
        out = text
        for a0, a1, rep in reversed(pieces):
            out = out[:a0] + rep + out[a1:]
        out = out[:start] + out[end + (len(out) - len(text)):] if False else out
        # drop the function-scope declaration (indices unchanged: it is before
        # every arm, and the arms were rewritten back to front)
        out = out[:start] + out[start:].replace(decl, "", 1)
        yield ("declare %s per switch arm" % name, out)


def t_volatile(text):
    """Make one pointer dereference `volatile`.

    A volatile load is never forwarded by cse and never deleted, which is the
    only way to keep a SECOND load of a word the source has already read --
    the shape a listing shows when it loads the same field twice with nothing
    in between. matching-levers has it as section 5; it landed func_80036F24,
    func_80037090, func_80037394 and the word count of func_800565D8, and it
    is the one documented lever search.py could not reach.

    Tried one site at a time. A plain STORE is skipped, because volatile on
    one pins stores the target schedules freely and reliably costs words -- but
    a compound assignment (`+=`, `|=`) is NOT skipped, deliberately: there the
    qualifier keeps the separate load AND the separate store, which is the
    shape matching-levers section 5 records for func_80054B08's six stores,
    and it is what took func_80047054 from 30 differing words to 24 on the
    first park this family was run against. The greedy loop composes several
    sites if several help.
    """
    for m in scan(r"\*\((u8|s8|u16|s16|u32|s32) \*\) ", text):
        if text[m.start() - 1 : m.start()] == '&':
            continue
        # a read, not a store: the next `=` must not be this deref's own
        tail = text[m.end():m.end() + 80]
        if re.match(r"\([^;=]*\)\s*=[^=]", tail):
            continue
        new = (text[:m.start()] + '*(volatile %s *) ' % m.group(1)
               + text[m.end():])
        yield ("volatile on the %s read at %d" % (m.group(1), m.start()), new)


def _retype_uses(body, sym, to_array):
    """Rewrite every use of `sym` in `body` for the other declared size.

    scalar -> array: `&SYM` becomes `SYM` and a bare `SYM` becomes `SYM[0]`.
    array -> scalar: `SYM[0]` becomes `SYM` and a bare `SYM` -- an array name
    decaying to a pointer -- becomes `&SYM`. Any other subscript is left
    verbatim, spacing included.

    ONE pass, deliberately. Doing the `[0]` rewrite first and the bare-name
    rewrite second turns `SYM[0] != 0` into `&SYM != 0`, which is a constant
    and deletes the test -- a silent semantic break that scored BETTER on
    func_8005914C (203 words against 205) because it removed a comparison.
    """
    def one(m):
        pre, sub = m.group(1), m.group(2)
        if sub is None:                       # a bare name
            if to_array:
                return sym if pre == '&' else sym + '[0]'
            return m.group(0) if pre == '&' else '&' + sym
        if sub.rstrip().endswith('['):        # SYM[something else]
            return m.group(0)
        if to_array:                          # already SYM[0]
            return m.group(0)
        return pre + sym                      # SYM[0] -> SYM

    return re.sub(r"(&?)\b%s\b(\s*\[\s*0\s*\]|\s*\[)?" % sym, one, body)


def t_global_decl(text):
    """Flip a GLOBAL's declared size between the scalar and incomplete-array
    spellings, rewriting its uses to match.

    This is matching-levers section 6's "declaration decides addressing" as a
    sweep. The two spellings differ in the INSTRUCTIONS, not just the
    relocation: a scalar sets SYMBOL_REF_FLAG, so cc1psx emits a bare-symbol
    load or an `la` and the `lui` and the access share ONE register, while an
    incomplete array gives `lui %hi` into its own register and `%lo` in the
    access's displacement. That decides whether the pair can straddle a
    branch, which register each half gets, and how many registers an address
    costs across a call.

    It landed func_800353F4 (the scalar form's single-register `lui`/`lw`
    cannot put the `lui` in the `beqz`'s delay slot; the array form can -- two
    words, and the park had tried a `u8 *` type but not this) and is half of
    func_8004DF8C (where the SCALAR form is the right one, for the
    `%gp_rel` store beside a `-4($sN)` displacement).

    Only file-scope `extern` declarations in the candidate itself are
    rewritten -- a global declared in a header is left alone, since changing
    it would change every other unit too.
    """
    # NOT `scan`: these declarations are at FILE scope, outside the function
    # body SPAN that every other family is restricted to. The `^extern`
    # anchor is what keeps this out of function bodies, which are indented.
    for m in re.finditer(r"(?m)^extern\s+((?:volatile\s+)?\w+(?:\s*\*)*)\s+(\w+)\s*(\[\s*\])?\s*;[ \t]*$",
                         text):
        ty, sym, arr = m.group(1), m.group(2), m.group(3)
        to_array = arr is None
        decl = "extern %s %s%s;" % (ty, sym, "[]" if to_array else "")
        body = text[: m.start()] + "\0" * len(decl) + text[m.end():]
        body = _retype_uses(body, sym, to_array)
        body = body.replace("\0" * len(decl), decl, 1)
        if body == text:
            continue
        yield ("global %s: %s -> %s" % (sym, "scalar" if to_array else "array",
                                        "array" if to_array else "scalar"),
               body)


def _match_brace(text, i):
    """Index just past the `}` that closes the `{` at text[i]."""
    assert text[i] == '{'
    d = 0
    while i < len(text):
        if text[i] == '{':
            d += 1
        elif text[i] == '}':
            d -= 1
            if d == 0:
                return i + 1
        i += 1
    return -1


def t_goto_loop(text):
    """Rewrite a `while`/`for` loop as a guarded BACKWARD GOTO.

    A backward goto is not a loop to loop.c at all -- the front end only emits
    NOTE_INSN_LOOP_BEG for a loop statement -- so the loop pass never records
    a biv, never strength-reduces, and never hoists an invariant out of it.
    That is matching-levers section 2, and it is the shape to reach for
    whenever the target recomputes an index shift every iteration where the
    build walks a pointer: it answered func_8003C67C's sweep row (two pointers
    walked by -8 against the listing's two `sll`s) and took func_8004E9EC from
    34 words to 32 by killing the second, biased giv on its cursor.

    The body is wrapped in its own block after the label, so a body that
    begins with declarations stays legal C89. The condition is duplicated, so
    a condition with a side effect would change meaning -- those are skipped.
    """
    for m in scan(r"(?m)^([ \t]*)(while|for)[ \t]*\(", text):
        ind, kw = m.group(1), m.group(2)
        # the condition: everything up to the matching ')'
        i = text.index('(', m.start(2))
        d = 0
        j = i
        while j < len(text):
            if text[j] == '(':
                d += 1
            elif text[j] == ')':
                d -= 1
                if d == 0:
                    break
            j += 1
        head = text[i + 1:j]
        k = text.find('{', j)
        if k < 0 or text[j + 1:k].strip():
            continue
        end = _match_brace(text, k)
        if end < 0:
            continue
        body = text[k + 1:end - 1]
        if kw == 'for':
            parts = head.split(';')
            if len(parts) != 3:
                continue
            init, cond, step = (x.strip() for x in parts)
            if not cond:
                continue
            pre = (ind + init + ';\n') if init else ''
            tail = (ind + '    ' + step + ';\n') if step else ''
        else:
            cond, pre, tail = head.strip(), '', ''
            if not cond:
                continue
        if any(t in cond for t in ('++', '--', '=')) and '==' not in cond \
                and '!=' not in cond and '<=' not in cond and '>=' not in cond:
            continue                      # a side effect in the condition
        lab = 'zzloop%d' % m.start()
        new = (pre
               + ind + 'if (' + cond + ') {\n'
               + ind + lab + ':\n'
               + ind + '    {' + body + ind + '    }\n'
               + tail
               + ind + '    if (' + cond + ') {\n'
               + ind + '        goto ' + lab + ';\n'
               + ind + '    }\n'
               + ind + '}\n')
        yield ("%s loop at %d -> backward goto" % (kw, m.start()),
               text[:m.start()] + new + text[end:])


TRANSFORMS = [
    t_decl_width,
    t_drop_cast16,
    t_add_cast16,
    t_split_binop,
    t_join_binop,
    t_cmp_swap,
    t_mask1,
    t_dup_into_arms,
    t_decl_order,
    t_decl_per_arm,
    t_volatile,
    t_global_decl,
    t_goto_loop,
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

    # A declaration NARROWING can change the VALUE a local holds, not just its
    # representation, and this score cannot see that: on 2026-10-27 a sweep of
    # all forty parks proposed nine changes and FOUR of them were narrowings
    # that scored better while being wrong -- an s16 holding an address that is
    # then split into four bytes, a u16 holding a pointer, an s16 taking an
    # allocator's u32 return. Each looked like progress and was a semantic
    # break. So say so, loudly, rather than leaving it to be noticed.
    WIDTH_RANK = {"s8": 1, "u8": 1, "s16": 2, "u16": 2, "s32": 4, "u32": 4}
    narrowings = []
    for lbl, before, after in accepted:
        m = re.match(r"decl (\w+): (\w+) -> (\w+)$", lbl)
        if m and WIDTH_RANK.get(m.group(3), 9) < WIDTH_RANK.get(m.group(2), 0):
            narrowings.append(m)
    if narrowings:
        print()
        print("REVIEW REQUIRED -- %d of these NARROW a declaration:"
              % len(narrowings))
        for m in narrowings:
            print("  %s: %s -> %s" % (m.group(1), m.group(2), m.group(3)))
        print(wrap("A narrowing changes the VALUE the local holds, and the "
                   "score cannot tell that from a shape change. Check each one "
                   "against the target listing before keeping it: if the "
                   "listing shifts, masks or stores the value at a width "
                   "wider than the new type, or the local takes a pointer or "
                   "another function's wider return value, the change is a "
                   "semantic break that happens to score well. Four of nine "
                   "accepted narrowings were exactly that on 2026-10-27.",
                   "  "))
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except BrokenPipeError:
        # piped into head/less: exit quietly rather than dumping a traceback
        os.dup2(os.open(os.devnull, os.O_WRONLY), sys.stdout.fileno())
        sys.exit(0)
