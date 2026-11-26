#!/usr/bin/env python3
"""parkresidue.py [--jobs N] [FUNC...] -- MEASURE every park, don't trust its header.

A park header states the residue its author measured. That number goes stale:
the unit changes underneath it as declarations are added and neighbours land,
and a header can simply be wrong. On 2026-11-16 I ranked a batch by scraping
headers with a regex and told an agent a park was at 6 differing when it was
at 33 -- the regex had matched a number from the prose.

This splices each park's body into the CURRENT unit that carries its stub
(either block: `csrc/ovl_dungeon*.c' or `csrc/ovl_dng_*.c') and runs
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
import io
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import parksplice  # noqa: E402

PROJ = os.environ.get("AZURE_ROOT", "/home/case/projects/azure")
UNIT = os.path.join(PROJ, "csrc/ovl_dungeon.c")

# ovl_dungeon.c was CARVED on 2026-11-20: the block is four translation units
# now, because four of its jump tables sit at a 4-mod-8 rodata offset and
# cc1psx writes `.align 3` before every table it compiles. A park's stub
# therefore lives in whichever unit owns its function, and its INCLUDE_ASM
# path NAMES that unit -- so neither the unit path nor the stub string can be
# hardcoded any more. Without this every park in a carved unit reports SKIP,
# which looks like a malformed park rather than a stale tool.
UNIT_GLOB = os.path.join(PROJ, "csrc/ovl_dungeon*.c")

# AND DUNGEON.BIN's 116 CARVED OBJECTS ARE A SECOND SET, with their own park
# directory. Both of these were hardcoded to the pre-carve layout, and the
# cost of that was not a crash but SILENCE: every `ovl_dng_*' park was
# unreachable, so the one tool whose whole job is "do not trust a park
# header" never looked at them. Measured on 2026-11-23, batch 12, both were
# stale by a wide margin and in the SAME direction:
#
#     func_s14_80024AE4   header 65 aligned,  actually 42
#     func_s27_80024E64   header 152 aligned, actually 94
#
# Both got better on their own, because siblings landed in their units and
# brought declarations and callee prototypes with them. A park header is a
# measurement of a unit that no longer exists, and in both cases a dispatcher
# reading the header would have mispriced the park by 50%.
PARK_DIRS = ("docs/dng-parks", "docs/dungeon-parks")
UNIT_GLOBS = (os.path.join(PROJ, "csrc/ovl_dungeon*.c"),
              os.path.join(PROJ, "csrc/ovl_dng_*.c"))


def unit_paths():
    import glob
    out = []
    for g in UNIT_GLOBS:
        out += glob.glob(g)
    return sorted(out)


def park_path(func):
    """The park file for FUNC in whichever directory holds it."""
    for d in PARK_DIRS:
        p = os.path.join(PROJ, d, "%s.c" % func)
        if os.path.exists(p):
            return p
    return os.path.join(PROJ, PARK_DIRS[0], "%s.c" % func)


def unit_of(func):
    """(path, unit-name) of the unit whose INCLUDE_ASM carries FUNC."""
    for path in unit_paths():
        stem = os.path.splitext(os.path.basename(path))[0]
        with open(path, encoding="utf-8") as f:
            if 'INCLUDE_ASM("asm/nonmatchings/%s", %s);' % (stem, func) in f.read():
                return path, stem
    return None, None


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


# A leading token that could really be a C return type. The FALLBACK path --
# scraping declaration-shaped lines out of prose when a park has no explicit
# DECLARATIONS block -- needs this, because the two-or-three-token rule alone
# accepts an English phrase in front of a function CALL: "It needs
# func_8009AF18(a, b, c, d);" parses as a prototype named `needs' returning
# `It'. One such line was spliced into the unit on 2026-11-16 and the compile
# failed with `unterminated character constant` at a line number inside the
# unit -- a failure that looks nothing like a park problem.
TYPEWORD = re.compile(
    r"^(void|char|int|short|long|float|double|signed|unsigned|const|static|"
    r"[us](8|16|32|64)|Unk\w*|[A-Z]\w*_t|struct|union|enum)\b")


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
    explicit = m is not None
    out = []
    # A TYPEDEF SPANS LINES AND THIS USED TO BE LINE-ORIENTED, so an invented
    # struct was dropped silently: none of `typedef struct X', `{',
    # `u8 b[0x8];' or `} X;' is a declaration-shaped LINE, and the park's
    # body then failed to compile with `UnkDngB12Name8' undeclared -- which
    # the permuter reports only as `base=?' with zero iterations, the same
    # symptom as a dead permuter. Accumulate a brace-balanced statement
    # first, and emit it whole.
    lines, i = [l for l in region.split("\n")], 0
    pending, depth = [], 0
    for raw in lines:
        line = raw.strip()
        if line.startswith("*"):
            line = line[1:].strip()
        if pending or re.match(r"^(typedef|struct|union|enum)\b", line):
            pending.append(line)
            depth += line.count("{") - line.count("}")
            if depth <= 0 and line.endswith(";"):
                block = "\n".join(pending)
                if block not in out:
                    out.append(block)
                pending, depth = [], 0
            continue
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
            # In the fallback path the first token must look like a type.
            if not explicit and not TYPEWORD.match(line):
                continue
        out.append(line)
    # THE FALLBACK PATH IS SILENT AND IT CAN SPLICE A FOREIGN PROTOTYPE.
    #
    # On 2026-11-22 an agent added a prose table of OTHER parks' missing
    # prototypes to func_800ADE74's header, with semicolons. Those lines are
    # declaration-shaped and that park has no explicit DECLARATIONS block, so
    # the fallback harvested all of them: parksplice reported 43 declaration
    # lines where 17 were meant and spliced fourteen other units' prototypes
    # into ovl_dungeon_800A4B88.c -- among them func_800B1768, func_800BC318,
    # func_800C0180 and func_800C27F0, any of which could reach a LANDED
    # function in that unit and change its codegen.
    #
    # The body still measured its expected 30 rows, so nothing failed and
    # nothing warned. It was caught only by someone reading parksplice's own
    # declaration count and finding it implausible.
    #
    # So say so. The threshold is deliberately low: a park that genuinely needs
    # more than a handful of declarations should name them in an explicit
    # DECLARATIONS block, which is what the block is for.
    if out and not explicit:
        print("parkresidue: %s has NO `DECLARATIONS' block, so %d "
              "declaration-shaped line(s) were harvested from its header prose."
              % (os.path.basename(path), len(out)), file=sys.stderr)
        if len(out) > 6:
            print("parkresidue: *** THAT IS A LOT. Prose that merely looks like "
                  "C is harvested too, and a foreign prototype spliced into the "
                  "unit can change a LANDED function's codegen. Check the list, "
                  "or give the park an explicit DECLARATIONS block.",
                  file=sys.stderr)
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
#   * a line containing `__asm__` is never a prototype to ADOPT. It is a
#     symbol alias -- `extern u16 D_800DD25A __asm__("D_800DD258 + 2");` or
#     `s32 f_i(u16, u16) __asm__("f");` -- and the unit never has one to
#     replace, so it must be INSERTED. Without this exclusion the lazy prefix
#     ate `u16 D_800DD25A ` and `(\w+)` captured **`__asm__`** as the
#     function name; adopt() then went looking for "the unit's prototype for
#     __asm__", matched an unrelated aliased declaration and replaced it,
#     and the compile failed with an undeclared identifier 7000 lines away in
#     a different function. Found 2026-11-16 by the agent it happened to.
PROTO = re.compile(
    r"^(?!typedef\b)(?![^;]*__asm__)[A-Za-z_][\w \*]*?\**(\w+)"
    r"\s*\([^;{()]*\)\s*;\s*$")


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
    # AFTER THE CARVE (2026-11-20) the unit's top block is one `#include` of
    # csrc/ovl_dungeon_decls.h, so the old `extern u8 D_80082E80[];` anchor is
    # in the HEADER and not in the unit at all. With it gone the only
    # placement left was just-above-the-body, which is too late for a callee
    # PROTOTYPE -- gcc then reports "type mismatch with previous implicit
    # declaration" against a call hundreds of lines above, and three parks
    # went from measuring to SKIP for that reason alone. The include line is
    # the new top block; try it first.
    inc = '#include "ovl_dungeon_decls.h"\n'
    if inc in spliced:
        yield spliced.replace(inc, inc + "\n" + decls, 1)
    anchor = "extern u8 D_80082E80[];\n"
    if anchor in spliced:
        yield spliced.replace(anchor, anchor + decls, 1)
    yield unit.replace(stub, decls + "\n" + body.rstrip() + "\n")


def measure(func):
    park = park_path(func)
    body = body_of(park, func)
    if body is None:
        return func, None, "no definition line in park"
    upath, uname = unit_of(func)
    if upath is None:
        return func, None, "no unit carries a stub (landed already?)"
    unit = open(upath, encoding="utf-8").read()
    stub = 'INCLUDE_ASM("asm/nonmatchings/%s", %s);' % (uname, func)
    if unit.count(stub) != 1:
        return func, None, "not a stub in the unit (landed already?)"
    d = tempfile.mkdtemp(prefix="parkres-")
    src = os.path.join(d, os.path.basename(upath))
    decls = decls_of(park) + preamble_of(park, func)
    # DROP ANY DECLARATION THE UNIT ALREADY CARRIES -- shared with parksplice,
    # which had this filter as a closure while this tool did not. A measured
    # ranking of 40 parks SKIPPED FOUR of them for `redefinition of struct
    # Unk...' while parksplice spliced the same parks cleanly and reproduced
    # their stated figures: a sibling landing adds a typedef to the unit, the
    # park still declares it, and the park looks stale when it is fine.
    decls, _dropped = parksplice.drop_known(decls, unit, func, quiet=True)
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
    if decls and not DECL_RE.search(
            open(park, encoding="utf-8").read().split("*/")[0]):
        err += "  [no DECLARATIONS block -- declarations were SCRAPED from prose]"
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
        for d in PARK_DIRS
        for p in glob.glob(os.path.join(PROJ, d, "func_*.c")))
    rows, bad = [], []
    with cf.ThreadPoolExecutor(max_workers=jobs) as ex:
        for func, res, err in ex.map(measure, funcs):
            (bad if err else rows).append((func, err) if err else (res, func))
    rows.sort()

    # A HELD PARK MUST NOT READ AS THE NEXT THING TO PICK UP. Three bodies in
    # this project cannot measure 0 for reasons outside the C -- splat
    # mislabelling a rodata word as the first instruction, a 4-byte-aligned
    # jump table, a table in asm/data/. Their park headers say so in their first
    # lines, but THIS RANKING is what anyone scans to choose work, and
    # func_b29_8017088C sat twelfth at "5 rows" with nothing to warn a picker.
    # Carry the park's own warning into the row.
    HELD = re.compile(r"CANNOT VERIFY AT 0|CANNOT MEET THE GATE|^\s*HELD:",
                      re.M)

    def _held(func):
        try:
            t = io.open(os.path.join(PROJ, "docs", "dng-parks", func + ".c"),
                        encoding="utf-8", errors="replace").read(4000)
        except OSError:
            return False
        return bool(HELD.search(t))

    print("%-18s %6s  %s" % ("function", "rows", "words"))
    for (nd, built, tgt), func in rows:
        flag = "" if built == tgt else "   <- %+d words" % (built - tgt)
        if _held(func):
            flag += "   *** HELD: cannot reach 0, see the park's first lines"
        print("%-18s %6d  %d/%d%s" % (func, nd, built, tgt, flag))
    for func, err in bad:
        print("%-18s   SKIP  %s" % (func, err))
    print("\n%d measured, %d skipped" % (len(rows), len(bad)))


if __name__ == "__main__":
    main()
