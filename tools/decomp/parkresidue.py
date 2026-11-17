#!/usr/bin/env python3
"""parkresidue.py [--jobs N] [FUNC...] -- MEASURE every park, don't trust its header.

A park header states the residue its author measured. That number goes stale:
the unit changes underneath it as declarations are added and neighbours land,
and a header can simply be wrong. On 2026-11-16 I ranked a batch by scraping
headers with a regex and told an agent a park was at 6 differing when it was
at 33 -- the regex had matched a number from the prose.

This splices each park's body into the CURRENT csrc/ovl_dungeon.c and runs
aligndiff, so the ranking is a measurement. It also catches a park that no
longer compiles, which is the more valuable failure to find early.

    python3 tools/decomp/parkresidue.py                  # all parks, ranked
    python3 tools/decomp/parkresidue.py func_800A2424    # just one
"""
import concurrent.futures as cf
import glob
import os
import re
import subprocess
import sys
import tempfile

PROJ = os.environ.get("AZURE_ROOT", "/home/case/projects/azure")
UNIT = os.path.join(PROJ, "csrc/ovl_dungeon.c")


DECL_RE = re.compile(
    r"^\s*DECLARATIONS\b[^\n]*\n(.*?)^\s*END DECLARATIONS\s*$",
    re.M | re.S)

# A line that is plausibly a C declaration: an extern, a typedef, or a
# function prototype. Agents write these inside prose, so matching the shape
# is more reliable than demanding an exact marker format -- the first version
# of this tool required a bare "DECLARATIONS" line and silently found nothing
# in a park whose block was headed "DECLARATIONS THIS BODY NEEDS (new; ...):".
# A prototype needs a RETURN TYPE -- at least two words before the "(".
# Without that rule this also matched `func_80094270(p, b, c, u, j);` quoted
# in a park's prose, i.e. a CALL, which then re-declared the callee wrongly.
# A one-line `typedef` is its own case: a function-POINTER typedef such as
# `typedef u8 *(*UnkDungeonSpawner)(s32, u8, u8, s16);` has a paren before the
# name, so the prototype alternative below cannot match it and the two-or-three
# token rule would reject it anyway. func_800B6AC4 used one and declared it
# nowhere, which cost the park its measurement twice over.
DECL_LINE = re.compile(
    # `.+;` and not `[^;]+;`: a one-line struct typedef has semicolons
    # INSIDE it, one per field, and stopping at the first means the line never
    # matches at all.
    r"^\s*(typedef\s+.+;"
    r"|extern\s+[\w ]+[\w\*\[\]]+\s*(\[\s*\])?\s*;"
    r"|\w+[\w \*]*\s+\**\w+\s*\([^;{]*\)\s*(__asm__\s*\([^)]*\))?\s*;)\s*$")


def decls_of(path):
    """Extract the declarations a park's body needs from its header.

    A park body is NOT self-contained: it needs externs, typedefs and callee
    prototypes that live in the unit's top block. Those were historically
    reported only in the agent's hand-back message -- a transcript, not the
    repo -- so 19 of 34 parks could not be compiled from the repo alone on
    2026-11-16.

    Preferred form, inside the header comment:

        DECLARATIONS
        extern u8 D_800E0979[];
        void func_80067E2C(void *, void *);
        END DECLARATIONS

    A trailing description on the DECLARATIONS line is fine. Failing that,
    every declaration-shaped line in the header comment is collected, so a
    park that merely lists them in prose still works.
    """
    s = open(path, encoding="utf-8").read()
    end = s.find("*/")
    header = s[:end] if end > 0 else s
    m = DECL_RE.search(header)
    region = m.group(1) if m else header
    out = []
    for line in region.split("\n"):
        line = line.strip()
        if line.startswith("*"):
            line = line[1:].strip()
        if not DECL_LINE.match(line) or line in out:
            continue
        if not line.startswith(("extern", "typedef")) and "(" in line:
            # A prototype is "<type> <name>(" -- two or three tokens before
            # the paren. Without this cap an ordinary English sentence that
            # ends in "(...);" parses as one; a park's prose line "Its third
            # parameter is the one it uses (UnkSlot *s, spilled to ...);"
            # was extracted and re-declared, and the compile failed on the
            # word "spilled".
            lead = line.split("(")[0].split()
            if not 2 <= len(lead) <= 3:
                continue
        out.append(line)
    return "".join(l + "\n" for l in out)


def preamble_of(path, func):
    """Top-level text a park puts BETWEEN its header comment and its body.

    `decls_of` reads declaration-SHAPED lines out of the header comment, one
    per line. That cannot carry a multi-line construct, so a park needing a
    `typedef struct { ... } Unk...;` writes it at top level below the comment
    instead and says "needs the typedef below" -- which is the natural place
    for it, and was silently dropped.

    The cost of dropping it is misleading: old gcc treats `UnkFoo *w;` with
    UnkFoo unknown as a syntax error and then DISCARDS THE REST OF THE
    DECLARATION BLOCK, so the first complaint is `c undeclared` about a local
    three lines further down that is declared perfectly well. func_8009A3D0
    skipped for that reason, and the error named the wrong line and the wrong
    identifier.

    Anything inside an `#if 0` alternative is left behind with it.
    """
    s = open(path, encoding="utf-8").read()
    end = s.find("*/")
    start = s.find("*/") + 2 if end > 0 else 0
    stop = _body_offset(s, func)
    if stop is None:
        return ""
    cond = re.compile(r"^\s*#\s*(if|ifdef|ifndef|endif)\b")
    depth, out = 0, []
    for line in s[start:stop].splitlines():
        c = cond.match(line)
        if c:
            depth += -1 if c.group(1) == "endif" else 1
            continue
        if depth == 0 and line.strip():
            out.append(line)
    return "".join(l + "\n" for l in out)


def _body_offset(s, func):
    """Offset of the LIVE definition of FUNC in park text S, or None."""
    defn = re.compile(r"^[A-Za-z_][\w \*]*\b%s\s*\([^;]*\)\s*$" % re.escape(func))
    cond = re.compile(r"^\s*#\s*(if|ifdef|ifndef|endif)\b")
    depth, m, off = 0, None, 0
    for line in s.splitlines(keepends=True):
        c = cond.match(line)
        if c:
            depth += -1 if c.group(1) == "endif" else 1
        elif depth == 0 and defn.match(line.rstrip("\n")):
            m = off
        off += len(line)
    return m


def body_of(path, func):
    """The park's LIVE definition of FUNC -- the last one OUTSIDE any #if 0.

    A park records rejected and alternative spellings under `#if 0`, and those
    blocks come AFTER the live body.  Taking the last definition in the file
    therefore grabbed the alternative, and spliced it together with its
    trailing `#endif` and no opener: `unbalanced #endif`, which is how
    func_8008E700 and func_800C0404 both stopped measuring on 2026-11-16.
    Taking the FIRST would be wrong too -- a park may lead with an older body
    under `#if 0` -- so track conditional depth and take the last at depth 0.
    """
    s = open(path, encoding="utf-8").read()
    m = _body_offset(s, func)
    if m is None:
        return None
    return s[m:]


# A prototype, and ONLY a prototype. Two traps, both of which fired:
#
#   * `typedef` is excluded outright. Without that, a function-pointer typedef
#     is a prototype-shaped line and `adopt` happily rewrites it.
#   * the parameter list may not itself contain parentheses. With `[^;{]*`
#     there, `extern void (*D_800DD830[])(void);` matched with the captured
#     NAME equal to `void` -- the lazy prefix ate `xtern `, `(\w+)` took
#     `void`, and the `[^;{]*` swallowed `*D_800DD830[])(void`. `adopt` then
#     looked for "the unit's prototype for void", found
#     `typedef void (*UnkDungeonStep)(u8 *, UnkVec *, u8 *, u8 *);` and
#     replaced it. Two parks that had measured fine stopped compiling, at a
#     line 30000 away from anything either of them touched.
#
# The cost is that a prototype with a function-pointer PARAMETER is no longer
# recognised as one. That is the safe direction: it is then inserted verbatim
# instead of adopted.
PROTO = re.compile(
    r"^(?!typedef\b)[A-Za-z_][\w \*]*?\**(\w+)\s*\([^;{()]*\)\s*;\s*$")


def reproto(text):
    """Drop duplicate prototypes, keeping the LAST spelling of each name."""
    keep, seen = [], {}
    for line in text.split("\n"):
        m = PROTO.match(line.strip())
        if not m:
            if line.strip():
                keep.append(line)
            continue
        seen[m.group(1)] = line
    return "".join(l + "\n" for l in keep) + "".join(
        l + "\n" for l in seen.values())


def sig_key(decl):
    """A prototype's meaning, with parameter names and spacing removed.

    `void f(u8 *p, s32 c);` and `void f(u8 *, s32);` are the same declaration.
    Comparing the text instead would call every park a prototype change.
    """
    decl = decl.strip().rstrip(";")
    head, _, args = decl.partition("(")
    args = args.rsplit(")", 1)[0]
    out = []
    for a in args.split(","):
        a = a.strip()
        if a in ("", "void"):
            out.append(a)
            continue
        # Drop the parameter NAME, which is the last token when there is more
        # than one. Stripping "a trailing identifier" unconditionally eats the
        # type itself in `s32` -- `s32` IS an identifier -- so the count of
        # remaining tokens is what makes this safe.
        toks = re.sub(r"\*", " * ", a).split()
        if len(toks) > 1 and re.match(r"^[A-Za-z_]\w*(\[\s*\])?$", toks[-1]):
            arr = "[]" if toks[-1].endswith("]") else ""
            toks = toks[:-1] + ([arr] if arr else [])
        out.append("".join(toks))
    return (re.sub(r"\s+", "", head), tuple(out))


def adopt(unit, decls):
    """REPLACE the unit's own prototype for any name the park re-declares.

    A park body whose signature differs from the unit's declaration is a
    PROTOTYPE CHANGE, not a missing declaration, and inserting a second
    spelling gets `conflicting types for func_...` rather than a measurement.
    Two parks were invisible to the ranking for this reason: the unit carries
    `void func_800AFBFC();` and `void func_800B51C8();` -- empty-parameter,
    void-returning -- while both bodies return s32 and take three arguments.

    Returns (unit with those lines rewritten, the declarations still to
    insert). A prototype change is real work the CALLERS have to be measured
    against; this only makes the park measurable so the size of the prize is
    known before an agent is sent.
    """
    out = []
    for line in decls.split("\n"):
        m = PROTO.match(line.strip())
        if not m:
            if line.strip():
                out.append(line)
            continue
        name = m.group(1)
        here = re.compile(r"^[A-Za-z_][\w \*]*?\**%s\s*\([^;{]*\)\s*;[^\n]*$"
                          % re.escape(name), re.M)
        hit = here.search(unit)
        if hit:
            # Only rewrite when the signature really DIFFERS. A park's body
            # carries parameter NAMES and the unit's declaration usually does
            # not, so a naive replace rewrites a line that means exactly the
            # same thing -- which then shows up as gratuitous churn in the
            # diff an agent hands back, and invites merging a cosmetic change
            # along with a real one.
            if sig_key(hit.group(0)) != sig_key(line):
                unit = here.sub(line.strip().replace("\\", "\\\\"), unit,
                                count=1)
        else:
            out.append(line)
    return unit, "".join(l + "\n" for l in out)


def splices(unit, stub, body, decls):
    """The unit with BODY spliced in, once per declaration PLACEMENT to try.

    There is no single right place to put a park's declarations.

      * The top block (after `extern u8 D_80082E80[];`, around line 358) is
        right for a callee PROTOTYPE, which has to precede the unit's own
        earlier calls to that callee or gcc reports "type mismatch with
        previous implicit declaration" against a definition hundreds of lines
        above.
      * That same place is WRONG for a typedef built on one of the unit's own
        structs -- `UnkDungeonSPXY` is declared around line 1260, so a park
        struct with a `UnkDungeonSPXY unk_70;` field cannot go at 358. Those
        belong immediately above the body.

    func_800AFBFC and func_800B84E4 each failed for the second reason while
    the first placement is what most parks need, so try the top block and fall
    back to just-above-the-body rather than choosing.
    """
    sig = body.split("\n", 1)[0].strip()
    decls = reproto(decls + sig.rstrip("{ ") + ";\n")
    unit, decls = adopt(unit, decls)
    spliced = unit.replace(stub, body.rstrip() + "\n")
    if not decls:
        yield spliced
        return
    anchor = "extern u8 D_80082E80[];\n"
    if anchor in spliced:
        yield spliced.replace(anchor, anchor + decls, 1)
    yield unit.replace(stub, decls + "\n" + body.rstrip() + "\n")


def measure(func):
    park = os.path.join(PROJ, "docs/dungeon-parks/%s.c" % func)
    body = body_of(park, func)
    if body is None:
        return func, None, "no definition line in park"
    unit = open(UNIT, encoding="utf-8").read()
    stub = 'INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", %s);' % func
    if unit.count(stub) != 1:
        return func, None, "not a stub in the unit (landed already?)"
    d = tempfile.mkdtemp(prefix="parkres-")
    src = os.path.join(d, "ovl_dungeon.c")
    decls = decls_of(park) + preamble_of(park, func)
    env = dict(os.environ, AZURE_SCRATCH=os.path.join(d, "sp"))
    os.makedirs(env["AZURE_SCRATCH"], exist_ok=True)
    err = None
    for text in splices(unit, stub, body, decls):
        open(src, "w").write(text)
        r = subprocess.run([sys.executable, "tools/decomp/aligndiff.py", src, func],
                           capture_output=True, text=True, env=env, cwd=PROJ)
        m = re.search(r"built (\d+) words, target (\d+) words, (\d+) aligned", r.stdout)
        if m:
            return func, (int(m.group(3)), int(m.group(1)), int(m.group(2))), None
        lines = (r.stderr or r.stdout).strip().split("\n")
        lines = [l for l in lines if "error" in l.lower() or "undeclared" in l.lower()]
        err = (lines[-1][:90] if lines else "did not compile")
    return func, None, err


def main():
    argv = sys.argv[1:]
    jobs, args, skip = 4, [], False
    for i, a in enumerate(argv):
        if skip:
            skip = False
            continue
        if a == "--jobs":
            jobs = int(argv[i + 1])
            skip = True            # the VALUE is not a function name
        elif not a.startswith("-"):
            args.append(a)
    funcs = args or sorted(
        os.path.basename(p)[:-2]
        for p in glob.glob(os.path.join(PROJ, "docs/dungeon-parks/func_*.c")))
    rows, bad = [], []
    with cf.ThreadPoolExecutor(max_workers=jobs) as ex:
        for func, res, err in ex.map(measure, funcs):
            (bad if err else rows).append((func, err) if err else (res, func))
    rows.sort()
    print("%-18s %6s  %s" % ("function", "rows", "words"))
    for (nd, built, tgt), func in rows:
        flag = "" if built == tgt else "   <- %+d words" % (built - tgt)
        print("%-18s %6d  %d/%d%s" % (func, nd, built, tgt, flag))
    for func, err in bad:
        print("%-18s   SKIP  %s" % (func, err))
    print("\n%d measured, %d skipped" % (len(rows), len(bad)))


if __name__ == "__main__":
    main()
