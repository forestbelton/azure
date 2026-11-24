#!/usr/bin/env python3
"""Replicate a landed DUNGEON.BIN body across every copy of it.

`dngfam.py` establishes that 1199 of this block's 2575 carved stubs are exact
copies of a sibling -- the 116 objects statically link a shared library, so one
function appears relinked in up to 216 of them. This tool is the other half of
that: it takes ONE landed representative and writes the same body into every
other member of its family, renaming the symbols each copy relinked.

## How the rename is derived, and why it is not a guess

The two listings are the same instruction stream; only symbolic operands
differ. So the map is read off POSITIONALLY: walk both bodies together, and at
each index pair the symbols in the representative's operand text with the
symbols in the member's. `func_b01_801714D4 -> func_b02_80171094` falls out as
a fact about the two listings rather than an inference about addresses.

A symbol that maps to two different things, or an index whose two texts hold
different NUMBERS of symbols, makes the member refuse rather than guess.

## Nothing lands unverified

Every cloned member is measured with `ovlverify.py` against ITS OWN listing
before its unit is written. A member that does not come back `0 differing` is
reported and left as `INCLUDE_ASM`. That is what makes the hex-literal
normalisation in `dngfam.py` safe to have: two functions that differ only in
an immediate sit in one family there, and here they simply fail to verify.

## Declarations travel with the body

A body needs its unit's declaration block. The declarations the representative
unit carries are renamed the same way and inserted into each member's unit if
that unit does not already have them.

Usage:
    python3 tools/decomp/dngclone.py FUNC [--dry-run] [--src UNIT.c]
                                          [--only unit,unit] [-j N]
"""
import argparse
import concurrent.futures
import itertools
import os
import re
import shutil
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dngfam  # noqa: E402

PROJ = dngfam.PROJ
# Every symbol class a carved object can name: a sibling function, a data
# label, a jump table. Branch labels (.L...) never appear in C.
#
# THE TAG IS OPTIONAL HERE AND THAT IS NOT COSMETIC. A per-copy data symbol
# comes out tagged when it falls inside the object's own carved data
# subsegment (`D_b03_80173C74`) and UNTAGGED when it falls in the `bin` tail
# splat left after the text (`D_80175EA0`) -- one family had both spellings in
# the same instruction pair. A pattern requiring the tag matched only half of
# them, left the other half unrenamed, and would have cost 41 copies.
CSYM = re.compile(r"\b(?:func|D|jtbl)_[A-Za-z0-9_]*[0-9A-Fa-f]{8}\b")
ASYM = re.compile(r"(?:\.L|func_|D_|jtbl_)[A-Za-z0-9_]*[0-9A-Fa-f]{8}")


def unit_of(func):
    return "ovl_dng_" + func.split("_")[1]


def unit_path(func):
    return os.path.join(PROJ, "csrc", unit_of(func) + ".c")


def symbol_map(rep, member):
    """{rep symbol: member symbol}, read positionally off the two listings."""
    A = dngfam.body(dngfam.listing_path(rep), rep)
    B = dngfam.body(dngfam.listing_path(member), member)
    if len(A) != len(B):
        raise ValueError(f"{member}: {len(B)} words against {rep}'s {len(A)}")
    out = {rep: member}
    for i, (x, y) in enumerate(zip(A, B)):
        sx, sy = ASYM.findall(x), ASYM.findall(y)
        if len(sx) != len(sy):
            raise ValueError(f"{member}: word {i} has {len(sx)} symbols in "
                             f"{rep} and {len(sy)} here")
        for a, b in zip(sx, sy):
            if out.setdefault(a, b) != b:
                raise ValueError(f"{member}: {a} maps to both {out[a]} and {b}")
    return out


def rename(text, smap):
    """Apply `smap` to every carved symbol in `text`.

    A symbol the map does not mention is left alone, and that is NOT treated as
    a failure. Two things land in that case and only one of them is a problem:

    * a reference OUT of the object -- `func_800ABDBC` in the resident overlay
      -- which is the same symbol in every copy, so leaving it is right. Most
      of these pair with themselves above anyway; the ones that do not are
      named in the landed body's COMMENT rather than its code, where there is
      no instruction to pair them against.
    * a per-copy symbol the listings somehow did not witness, which is wrong
      and which `ovlverify` then catches, because a body carrying another
      copy's address cannot come back 0 differing.

    Refusing on an unmapped symbol instead was a real bug: it rejected every
    member of a family whose landed comment happened to cite a resident
    function by name. The verifier is the gate; this is bookkeeping.
    """
    unmapped = set()

    def sub(m):
        s = m.group(0)
        if s in smap:
            return smap[s]
        unmapped.add(s)
        return s
    return CSYM.sub(sub, text), unmapped


def def_match(src_text, func):
    """(match, index of the parameter list's `)`) for `func`'s DEFINITION.

    EVERY candidate is tried, and only a definition is accepted. A landed body
    elsewhere in the unit may FORWARD-DECLARE this function -- the landed
    func_b01_80172628 declares func_b01_80171C34 -- and the prototype comes
    first in the file, so taking the first regex match picked the declaration,
    then scanned on for the next `{` and swallowed every INCLUDE_ASM and
    definition between it and a balanced brace. The clone came out with seven
    duplicated stubs and the compiler said `redefinition of
    __maspsx_include_asm_hack_...', naming lines nobody had written.
    A definition has `{` after its parameter list; a prototype has `;`.
    """
    for cand in re.finditer(r"(?m)^[A-Za-z_][\w \*\t]*?\b%s\s*\("
                            % re.escape(func), code_view(src_text)):
        i, depth = cand.end() - 1, 0
        while i < len(src_text):
            if src_text[i] == "(":
                depth += 1
            elif src_text[i] == ")":
                depth -= 1
                if depth == 0:
                    break
            i += 1
        j = i + 1
        while j < len(src_text) and src_text[j] in " \t\r\n":
            j += 1
        if j < len(src_text) and src_text[j] == "{":
            return cand, i
    return None, None


def def_signature(src_text, func):
    """`func`'s own definition in `src_text`, rewritten as a prototype.

    WHY A CLONE CANNOT JUST CARRY THE REPRESENTATIVE'S DECLARATION OF A
    SIBLING. Two agents working different families pick their own parameter
    types for the same callee, and both are right for their own function:
    func_b01_80171C34's author declared its sibling `s32 f(u8 *, void *,
    void *, u8 *)` while that sibling's own landed body -- a member of another
    family -- is `s32 f(u8 *, UnkVec *, u8 *, u8 *)`. In the 26 units where
    both are present the renamed declaration is a `conflicting types' error.

    The definition is the authority, so the prototype is taken from it. A
    pointer argument converts without an instruction, so the call site's code
    is unchanged -- and every member is verified anyway.

    Returns None for a K&R definition, whose parameter list is names only and
    would make an invalid prototype.
    """
    m, close = def_match(src_text, func)
    if m is None:
        return None
    sig = src_text[m.start():close + 1]
    params = sig[sig.index("(") + 1:-1].strip()
    if params and params != "void" and not re.search(
            r"\*|\b(?:void|char|short|int|long|signed|unsigned|float|double|"
            r"struct|union|enum|[su]8|[su]16|[su]32|[su]64|f32|Unk\w*)\b",
            params):
        return None                      # K&R: `f(p, b, c, d)`
    return " ".join(sig.split()) + ";"


def fix_forward_decl(text, func):
    """Make any declaration of `func` in `text` agree with its definition.

    TWO AGENTS CAN DISAGREE ABOUT THE ARITY OF ONE FUNCTION and both land.
    func_b09_80171158 takes func_b09_80171320's ADDRESS and declared it with
    four parameters; func_b09_80171320's own body is three. Taking an address
    does not depend on the signature, so both verified at 0 differing in their
    own copies -- and the conflict appeared only when they met in one unit.

    The definition is the authority. This rewrites the stale declaration
    rather than deleting it, because something in the unit may need the
    forward reference, and the caller re-verifies every function the unit
    defines afterwards.
    """
    sig = def_signature(text, func)
    if not sig:
        return text
    start, end = decl_span(text)
    block, out = text[start:end], []
    changed = False
    for d in decl_block(text):
        mm = DECLARED.search(d)
        if mm and mm.group(1) == func and " ".join(d.split()) != sig:
            block = block.replace(d, sig, 1)
            changed = True
    return text[:start] + block + text[end:] if changed else text


def defined_funcs(text):
    """Every carved function `text` DEFINES, in source order."""
    out = []
    for m in re.finditer(r"(?m)^[A-Za-z_][\w \*\t]*?\b(func_[A-Za-z0-9]+_[0-9A-Fa-f]{8})\s*\(",
                         code_view(text)):
        name = m.group(1)
        if name not in out and def_match(text, name)[0] is not None:
            out.append(name)
    return out


def is_type_decl(decl):
    """True for a typedef / struct / union / enum / preprocessor statement.

    THESE MUST NEVER GO THROUGH `DECLARED`. It extracts the declared name by
    taking the first identifier that is not a type keyword, and in a
    `typedef struct X { u8 pad_00[0xC]; ... } X;` that is the FIRST FIELD. Two
    unrelated structs whose first field is `pad_00` therefore look like two
    declarations of one symbol, and `reconcile_decls` substituted
    agent-h4's `UnkDng708A4SP` with the unit's existing `UnkDngRoom2970` --
    after which the unit did not compile at all, with `UnkDng708A4SP'
    undeclared pointing at a line nobody had written that way.

    This is the third defect of exactly this shape (see the note on
    function-pointer arrays), so the rule is now a named predicate rather
    than a `startswith` repeated at each call site.
    """
    return decl_code(decl).startswith(("typedef", "struct", "union", "enum",
                                       "#"))


def reconcile_decls(decls, unit_text):
    """`decls` made compatible with what `unit_text` already says.

    Three cases, in order:

    * the unit DEFINES the symbol -- take the prototype from that definition
      (see def_signature), because the definition is the authority;
    * the unit already DECLARES it -- keep the unit's declaration and drop
      ours. Two agents pick their own parameter types for a shared callee and
      both are right for their own function, so `s32 f(u8 *, void *, u8 *,
      u8 *)` from one family meets `s32 f(u8 *, void *, void *, u8 *)` from
      another and the compiler says `conflicting types'. The one already in
      the unit is the one its other landed bodies compile against; a pointer
      argument converts without an instruction, so dropping ours changes no
      code, and the member is verified regardless.
    * otherwise keep ours.

    A TYPE STATEMENT IS PASSED THROUGH UNTOUCHED -- see is_type_decl. It
    declares no symbol the unit could define or redeclare, and matching one by
    "declared name" matches its first field.
    """
    have = {}
    for d in decl_block(unit_text):
        if is_type_decl(d):
            continue
        mm = DECLARED.search(decl_code(d))
        if mm:
            have[mm.group(1)] = d
    out = []
    for d in decls:
        if is_type_decl(d):
            out.append(d)
            continue
        mm = DECLARED.search(decl_code(d))
        if not mm:
            out.append(d)
            continue
        name = mm.group(1)
        sig = def_signature(unit_text, name)
        if sig:
            out.append(sig)
        elif name in have:
            out.append(have[name])
        else:
            out.append(d)
    return out


def definition(src_text, func):
    """(comment + definition, span) for `func` in a unit's source."""
    # EVERY candidate, and only the one that is a DEFINITION. A landed body
    # elsewhere in the unit may FORWARD-DECLARE this function -- the landed
    # func_b01_80172628 declares func_b01_80171C34 -- and the prototype comes
    # first in the file, so taking the first match picked the declaration,
    # then scanned for the next `{` and swallowed every INCLUDE_ASM and
    # definition between it and a balanced brace. The clone came out with
    # seven duplicated stubs and the compile said `redefinition of
    # __maspsx_include_asm_hack_...', naming lines nobody had written.
    # A definition has `{` after its parameter list; a prototype has `;`.
    m, _close = def_match(src_text, func)
    if m is None:
        return None, None
    start = m.start()
    # Pull in an immediately preceding comment block, which carries the
    # landing's word count and the spelling fact that made it land.
    #
    # LINE BY LINE, NOT BACK TO THE LAST BLANK LINE. The blank-line version
    # read "the text since the previous paragraph break", and when two landed
    # functions ended up adjacent with no blank line between them -- which the
    # splice below used to produce -- that paragraph reached back over the
    # WHOLE previous function. Its body was then cloned into every member of
    # the next family as a second definition, and the compiler said
    # `redefinition of func_b01_801735B4' from a file nobody had written by
    # hand. Walk up only over lines that are themselves comment.
    lines = src_text[:start].split("\n")
    i = len(lines) - 1
    if i >= 0 and lines[i] == "":
        i -= 1          # the newline ending the line before the definition
    end_i = i + 1
    while i >= 0:
        t = lines[i].strip()
        if t.startswith("//"):
            i -= 1
            continue
        if t.endswith("*/"):
            while i >= 0 and not lines[i].lstrip().startswith("/*"):
                i -= 1
            i -= 1
            continue
        break
    if i + 1 < end_i:
        start = sum(len(x) + 1 for x in lines[:i + 1])
    i = src_text.index("{", m.end() - 1)
    depth = 0
    while True:
        c = src_text[i]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                break
        i += 1
    return src_text[start:i + 1], (start, i + 1)


def decl_span(src_text):
    """(start, end) of the unit's declaration block: between the include and
    the first stub or definition."""
    m = re.search(r'#include "common\.h"\n', src_text)
    start = m.end() if m else 0
    end = len(src_text)
    for pat in (r"(?m)^INCLUDE_ASM\(", r"(?m)^[A-Za-z_][\w \*\t]*\w+\s*\([^;]*\)\s*\n\{"):
        mm = re.search(pat, src_text[start:])
        if mm:
            end = min(end, start + mm.start())
    return start, end


def decl_block(src_text):
    """The unit's declaration block, as individual statements."""
    start, end = decl_span(src_text)
    chunk = src_text[start:end]
    # STATEMENTS, NOT LINES. A declaration block can hold a multi-line
    # struct typedef, and splitting on newlines shreds it: `needed_decls`
    # keeps the `typedef struct X` line, drops the `{`, drops each field
    # (whose name is not in the body), and the clone then fails to compile
    # against a file nobody could read. Track brace depth and close a
    # statement only at depth 0.
    #
    # AND THE `;` MUST BE IN CODE. A declaration's comment is prose and prose
    # contains semicolons: agent-h13 wrote
    #
    #   /* ... func_s20_80025340 writes it, func_s20_80024EF4 reads it;
    #      D_800264F8 is &D_80026478[0x10], its second bank. */
    #
    # and the first line's `;` closed a statement MID-COMMENT. The remainder
    # became a statement of its own beginning `D_800264F8 is &...', the
    # typedef that followed was orphaned, and the unit would not compile:
    # `syntax error before `is''. Fourth time a tool here has read a comment
    # as code -- see code_mask in vary_consts, decl_code in needed_decls and
    # code_view in def_match.
    mask = code_mask(chunk)
    out, cur, depth, off = [], [], 0, 0
    for ln in chunk.split("\n"):
        t = ln.strip()
        if not t and not cur:
            off += len(ln) + 1
            continue
        cur.append(ln)
        depth += ln.count("{") - ln.count("}")
        closes = False
        if depth <= 0:
            if t.endswith("*/"):
                closes = True            # a comment block legitimately ends here
            elif t.endswith(";"):
                closes = mask[off + ln.rindex(";")]
        if closes:
            out.append("\n".join(cur).strip())
            cur, depth = [], 0
        off += len(ln) + 1
    if cur:
        out.append("\n".join(cur).strip())
    return out


# The identifier a declaration declares: the first one followed by `(`, `[`
# or `;` THAT IS NOT A TYPE KEYWORD. Covers `s16 func_X(void *, s16);`,
# `extern u8 D_Y[];`, `extern u16 D_Z;`, the alias form
# `u8 *f_r(u8 *) __asm__("f");` and the function-pointer array
# `extern void (*D_W[])(u8 *);`.
#
# THE KEYWORD SKIP IS NOT COSMETIC. In `extern void (*D_80174E5C[])(...)` the
# `void` is itself followed by `(`, so the un-skipped pattern named every such
# declaration `void` -- and `reconcile_decls`, which keys on the name, then
# treated a unit's existing `D_80174E4C` table declaration as the SAME symbol
# and substituted it for the one being cloned. The result was
# `D_80174E5C' undeclared, from a declaration that had been silently replaced
# by its neighbour.
_TYPEWORD = re.compile(
    r"^(?:void|char|short|int|long|signed|unsigned|float|double|const|"
    r"volatile|extern|static|register|struct|union|enum|typedef|"
    r"[us](?:8|16|32|64)|f32)$")
_DECL_CAND = re.compile(r"\b([A-Za-z_]\w*)\s*(?=\(|\[|;)")


class DECLARED:
    """`DECLARED.search(decl)` -> a match whose group(1) is the declared name."""

    @staticmethod
    def search(decl):
        for m in _DECL_CAND.finditer(decl):
            if not _TYPEWORD.match(m.group(1)):
                return m
        return None


def decl_code(decl):
    """`decl` with its LEADING COMMENTS stripped, for keyword tests.

    A hand-written declaration usually comes with a comment above it, and an
    invented typedef always does -- the comment IS the witness for the struct.
    So the statement's text starts `/*', not `typedef', and a keyword test on
    the raw text silently says it is not a typedef. That is how agent-h4's
    three typedefs were dropped from a clone candidate and the whole unit
    failed to compile with `UnkDng708A4SP' undeclared.
    """
    out = decl.lstrip()
    while True:
        if out.startswith("/*"):
            j = out.find("*/")
            if j == -1:
                return ""
            out = out[j + 2:].lstrip()
        elif out.startswith("//"):
            j = out.find("\n")
            if j == -1:
                return ""
            out = out[j + 1:].lstrip()
        else:
            return out


def needed_decls(decls, body):
    """The subset of `decls` the body actually refers to.

    WITHOUT THIS, A CLONE CARRIES ITS WHOLE UNIT'S DECLARATION BLOCK. The
    representative is read out of `csrc/<unit>.c`, which by the time a later
    family is cloned holds every earlier family's declarations too, and those
    get renamed against THIS family's symbol map -- which does not mention
    them, so they travel unchanged. That is how `void func_b01_801714D4();`
    came to be declared in csrc/ovl_dng_b55.c: harmless to the link, since an
    unused declaration emits nothing, and wrong in every other way.

    A typedef or struct definition is kept unconditionally: it is a type, so
    the body refers to it by a name this cannot see. Both tests run on
    `decl_code`, not the raw statement, because of the comment above it.

    AND THE SEARCH RUNS ON THE BODY'S CODE, NOT ITS COMMENT. A landed body's
    header names its twin -- "TWIN: the landed carved sibling
    func_b03_80171DA0" -- and matching that pulled the twin's declaration into
    every clone of the family. 32 declarations of another bank's function
    ended up across 28 units that way: harmless to the link, since an unused
    declaration emits nothing, and wrong in every other way, and they are
    exactly the sort of thing a later agent reads and believes. Same lesson as
    `code_mask` in vary_consts: a comment is prose, not a reference.
    """
    code = "".join(c for c, keep in zip(body, code_mask(body)) if keep)
    out = []
    for d in decls:
        if is_type_decl(d):
            out.append(d)
            continue
        m = DECLARED.search(decl_code(d))
        if m and re.search(r"\b%s\b" % re.escape(m.group(1)), code):
            out.append(d)
    return out


def splice(unit_text, func, body):
    """Replace `func`'s INCLUDE_ASM line with `body`."""
    # `[ \t]*$`, NOT `\s*$`. A greedy `\s*` before a MULTILINE `$` eats the
    # newline that ends the stub's own line, so the body landed flush against
    # the next definition and the blank line between them was gone -- which is
    # what made the comment walk-back above misread the file.
    pat = re.compile(r'(?m)^INCLUDE_ASM\("[^"]*",\s*%s\);[ \t]*$' % re.escape(func))
    if not pat.search(unit_text):
        raise ValueError(f"{func}: no INCLUDE_ASM stub in its unit")
    return pat.sub(lambda _: body.replace("\\", "\\\\"), unit_text, count=1)


def insert_decls(text, decls):
    """`text` with any of `decls` it lacks appended to its declaration block.

    AT THE END OF THE BLOCK, NOT AT THE TOP. Inserting after the `#include`
    put a new `extern UnkDngRoom2970 D_800E2970_s[]' AHEAD of the
    `typedef struct UnkDngRoom2970' the unit already carried further down, and
    the compile said `syntax error before D_800E2970_s' -- in three of one
    family's four members, since the fourth happened not to have the typedef
    yet and so received both in dependency order. Appending leaves every
    existing declaration in scope, and a newly added typedef still precedes a
    newly added user of it because `decls' keeps the source order it came in.
    """
    # COMPARE ON A WHITESPACE-NORMALISED FORM. An exact-text test misses a
    # declaration the unit already carries in a different wrapping -- which
    # is routine, because clang-format reflows a long prototype after a
    # landing and the next clone then adds its own copy. 11 duplicated
    # declarations had accumulated across 10 units that way; harmless to the
    # link, and agent-h17 had to point out that one of them was sitting twice
    # in a row in the file it was reading.
    norm = lambda d: " ".join(d.split())
    have = {norm(d) for d in decl_block(text)}
    add, seen = [], set(have)
    for d in decls:
        k = norm(d)
        if k in seen:
            continue
        seen.add(k)
        add.append(d)
    if not add:
        return text
    _, end = decl_span(text)
    return text[:end] + "\n".join(add) + "\n\n" + text[end:]


def add_body(text, member, body, decls):
    """`text` with `member`'s stub replaced by `body` and `decls` ensured."""
    return splice(insert_decls(text, decls), member, body)


TAGGED_LOCAL = re.compile(r"^func_(?!80)[A-Za-z0-9]+_[0-9A-Fa-f]{8}$")
NUM = re.compile(r"\b(0[xX][0-9a-fA-F]+|\d+)\b")
# An instruction's text with every numeric literal blanked, so two copies that
# differ only in an immediate compare equal.
_NUMBLANK = re.compile(r"-?\b(?:0[xX][0-9a-fA-F]+|\d+)\b")


def const_pairs(rep, member):
    """[(rep value, member value)] for immediates that differ between two
    family members whose instruction streams are otherwise identical.

    THREE OF THE FIRST TWENTY FAMILIES SPLIT ON ONE IMMEDIATE. `dngfam.py`
    normalises hex literals, so these sit in one family and `--diff` calls
    them the same -- which is right, because they ARE the same function: a
    per-bank actor-state constant, a per-bank angle window. The copies then
    failed to verify at exactly 1 differing and had to be re-cloned by hand
    from a sed of the body.

    This reads the pair off the two listings instead. Only an instruction
    whose text matches once the numbers are blanked contributes, so a real
    opcode or register difference is still a refusal rather than a guess --
    and the substituted body is verified against the member's own listing
    like any other clone.
    """
    A = dngfam.body(dngfam.listing_path(rep), rep)
    B = dngfam.body(dngfam.listing_path(member), member)
    out = []
    for x, y in zip(A, B):
        if x == y or _NUMBLANK.sub("#", x) != _NUMBLANK.sub("#", y):
            continue
        xs, ys = _NUMBLANK.findall(x), _NUMBLANK.findall(y)
        for a, b in zip(xs, ys):
            if a != b:
                out.append((int(a, 0), int(b, 0)))
    return out


PLACEHOLDER = re.compile(r"\b(WIP|TODO|FIXME|XXX|HACK|PLACEHOLDER)\b")


def placeholder_comment(body):
    """The marker text if `body`'s leading comment is a placeholder, else None.

    A GUARD AGAINST INTEGRATING A LIVE AGENT'S WORKING FILE. Twice now I have
    run dngland against an agent's copy on an interim "LANDED" message rather
    than waiting for its hand-back, and twice the bodies were right while the
    comments were still `/* WIP3 */' and `/* WIP func_b20_8017087C */'. The
    second time a CLONE inherited the marker, so it reached a unit no agent
    had ever opened.

    The comments are the most valuable thing the sweep produces after the
    code -- a later agent greps a twin's header for its measured facts -- so a
    placeholder is worse than nothing. Writing the rule in the docs did not
    stop me repeating it; this does.
    """
    i = body.find("*/")
    head = body[:i] if i != -1 else body[:400]
    m = PLACEHOLDER.search(head)
    return m.group(1) if m else None


def code_view(text):
    """`text` with every comment character replaced by a space.

    SAME LENGTH, so a match's offsets still index the original. This is for
    the patterns that look for a DEFINITION, which are line-anchored on
    `^[A-Za-z_]...name(` -- a comment line at column zero reading
    `func_b03_80170854(p, b, c, d) is the twin' matches that exactly, and
    `definition()` would then take the comment's span and splice over it,
    swallowing whatever lies between it and the next balanced brace. Nothing
    in csrc/ does that today; two of this tool's worst defects were
    mis-anchored spans from other causes, so the pattern is closed rather
    than watched.

    Third place the same rule has been needed, after code_mask in
    vary_consts and decl_code in needed_decls: a comment is prose, not code.
    """
    return "".join(c if keep else (" " if c != "\n" else "\n")
                   for c, keep in zip(text, code_mask(text)))


def code_mask(text):
    """[bool] per character: True where `text` is CODE, not comment.

    COMMENTS MUST BE EXCLUDED FROM THE LITERAL SEARCH. A landed body's header
    explains its constants in prose -- "the state-9 actor", "9 members" -- so
    the value being substituted reliably occurs twice and `vary_consts`
    refused every member of the first family it was tried on.
    """
    ok = [True] * len(text)
    i = 0
    while i < len(text):
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = len(text) if j == -1 else j + 2
            for k in range(i, j):
                ok[k] = False
            i = j
        elif text.startswith("//", i):
            j = text.find("\n", i)
            j = len(text) if j == -1 else j
            for k in range(i, j):
                ok[k] = False
            i = j
        else:
            i += 1
    return ok


def _tok(sample, value):
    """`value` spelled the way `sample` (the literal being replaced) is."""
    return ("%#x" % value) if sample.lower().startswith("0x") else str(value)


def _sites(body, ok, old):
    """[(match, kind)] where `old` occurs in the CODE of `body`.

    kind "plain" is the literal itself. kind "tilde" is the ONE-S COMPLEMENT
    SPELLING, and it is not a nicety: the assembler writes a clear-these-bits
    mask as a negative immediate (`addiu $v1, $zero, -0x9`) while the C that
    produces it is `&= ~8`. The first family this mattered on refused three
    of six members over exactly that, because -0x9 appears nowhere in the
    source text. A tilde match requires the `~` to be the token's immediate
    predecessor, so a bare `8` elsewhere in the body cannot be taken for one.
    """
    out = []
    for m in NUM.finditer(body):
        if not ok[m.start(1)]:
            continue
        v = int(m.group(1), 0)
        if v == old:
            out.append((m, "plain"))
            continue
        if old >= 0:
            continue
        j = m.start(1) - 1
        while j >= 0 and body[j] in " \t":
            j -= 1
        if j < 0:
            continue
        if v == ~old and body[j] == "~":
            out.append((m, "tilde"))
        elif v == -old and body[j] == "-":
            # `dx - 0x10' is `addiu -0x10' in the listing. The C literal is
            # POSITIVE and the sign is an operator, so the search for -0x10
            # found nothing and three members of func_b06_80174214's family
            # were refused with "appears only 0 times in the code". The
            # operator is part of the match: see _apply.
            out.append((m, "minus"))
    return out


def _apply(body, picks):
    """`body` with every (match, value, kind) in `picks` substituted.

    Right to left, so an earlier replacement cannot move a later match's
    offsets. Two different `old` values can never select the same match, so
    the picks are disjoint by construction.
    """
    for m, val, kind in sorted(picks, key=lambda x: -x[0].start(1)):
        if kind == "minus":
            # The sign lives in the OPERATOR, so flip it with the value:
            # -0x10 -> +0x10 rewrites `dx - 0x10' as `dx + 0x10'. The match
            # is extended back over the `-' and the space before it so the
            # whole thing is replaced at once.
            j = m.start(1) - 1
            while j >= 0 and body[j] in " \t":
                j -= 1
            assert body[j] == "-"
            sign = "-" if val < 0 else "+"
            body = (body[:j] + sign + body[j + 1:m.start(1)]
                    + _tok(m.group(1), abs(val)) + body[m.end(1):])
            continue
        v = val if kind == "plain" else ~val
        body = body[:m.start(1)] + _tok(m.group(1), v) + body[m.end(1):]
    return body


def vary_consts(body, pairs, limit=12):
    """([candidate body], [unsure]) -- bodies to try, best guess first.

    Matched BY VALUE, not by text: the listing says `0x9` where the C says
    `9`, and a clear-these-bits mask is a negative immediate in asm and a `~`
    in C (see _sites). Comments are excluded (see code_mask).

    `const_pairs` reports ONE ENTRY PER DIFFERING SITE, so a struct offset
    used twice in the body arrives as the same pair twice. k identical pairs
    against exactly k occurrences in the code is unambiguous: substitute all
    k. That alone recovered three members of func_b01_80173AAC's family, which
    an earlier one-occurrence-only rule had refused.

    WHEN THE VALUE OCCURS MORE OFTEN THAN IT DIFFERS, THE CHOICE IS RETURNED
    RATHER THAN REFUSED. func_b04_80174948 writes `p[0x9A] = 0x17' twice and
    func_b10_801751C0 differs at ONE of them, so which `0x17' becomes `0x18'
    is not knowable from a value -- but it is knowable from a compile. This
    returns every placement, first-occurrence-first, and the caller verifies
    them in turn. That is the sweep's rule everywhere else and it holds here:
    a wrong guess costs a failed verify, never a wrong landing. Capped at
    `limit` candidates so a pathological family cannot turn into a build farm.

    Still refused by name, because no enumeration helps:
      * one `old` wanting two different `new` values at different sites;
      * a value that differs at MORE sites than it appears in the code, which
        means the body does not spell it the way the listing does at all.

    Every substitution is recorded in the body's leading comment. Without that
    the clone's inherited write-up would describe the representative's
    constant while the code carried another -- a comment that is quietly wrong
    is worse than no comment, and these are the most valuable thing the sweep
    produces after the code.
    """
    want = {}
    for old, new in pairs:
        want.setdefault(old, []).append(new)

    ok = code_mask(body)
    unsure, choices, notes = [], [], []
    for old, news in want.items():
        if len(set(news)) != 1:
            unsure.append(f"{old:#x} wants {len(set(news))} different values "
                          f"({', '.join('%#x' % n for n in sorted(set(news)))})")
            continue
        new, need = news[0], len(news)
        hits = _sites(body, ok, old)
        if not hits:
            unsure.append(f"{old:#x} differs at {need} site(s) but does not "
                          f"appear in the code at all")
            continue
        if need >= len(hits):
            # MORE ASM SITES THAN C OCCURRENCES IS NORMAL AND UNAMBIGUOUS.
            # One C access can carry the immediate twice: `*(s32 *)(p + 0xA8)
            # += b->unk_14' is a load AND a store. func_b11_80172258 spells
            # 0xA8 four times and its listing has five, so an earlier
            # count-match rule refused all four members of the family. When
            # every differing site outnumbers the spellings, every spelling
            # moves -- that is what "differs at all of them" means.
            combos = [tuple(hits)]
        else:
            # Fewer differing sites than spellings: which ones is not knowable
            # from a value, so hand the caller every placement to verify.
            combos = list(itertools.combinations(hits, need))[:limit]
        choices.append([[(m, new, kind) for m, kind in combo]
                        for combo in combos])
        tilde = any(k == "tilde" for _, k in hits)
        notes.append(f"{old:#x} -> {new:#x}"
                     + (f" (as ~{~old:#x} -> ~{~new:#x})" if tilde else "")
                     + (f" at {need} of {len(hits)} sites"
                        if len(hits) > need else
                        f" at all {len(hits)} sites" if len(hits) > 1 else ""))
    # A DIFFERING IMMEDIATE THAT IS NOT IN THE SOURCE AT ALL MAY BE DERIVED
    # FROM ONE THAT IS. `*(UnkDngS58Name12 *)((u8 *)n + 0x58) = ...' writes
    # 0x58 in the C and emits lwl/lwr/swl/swr at 0x58, 0x5B, 0x5C, 0x5F, 0x60
    # and 0x63. A sibling whose base is elsewhere differs at all six, and five
    # of them appear nowhere in the source -- so substituting 0x58 is the
    # whole job and the other five are consequences of it. Both members of
    # func_s58_8002553C's family were refused over exactly that.
    #
    # The test is the DELTA: an unplaceable pair whose (new - old) matches a
    # pair we are already substituting is a derived field of that same
    # expression and needs nothing. Anything else is still refused, and the
    # verify still decides.
    if unsure:
        deltas = {n - o for o, news in want.items()
                  if len(set(news)) == 1 and _sites(body, ok, o)
                  for n in [news[0]]}
        left = []
        for msg in unsure:
            m = re.match(r"(-?0x[0-9a-f]+) differs at (\d+) site", msg)
            if m and "does not appear in the code at all" in msg:
                o = int(m.group(1), 16)
                if any((o + d) != o and d in deltas for d in deltas):
                    continue          # same delta as a substitution we make
            left.append(msg)
        unsure = left
    if unsure:
        return [], unsure

    note = ("   THIS COPY'S OWN CONSTANT: " + ", ".join(notes) +
            ".\n   The rest of this comment is the family "
            "representative's and its other\n   numbers are its own.\n")
    out = []
    for sel in itertools.islice(itertools.product(*choices), limit):
        cand = _apply(body, [pick for part in sel for pick in part])
        if notes:
            cut = cand.find("*/")
            if cut != -1:
                # The comment's last line usually has no trailing newline
                # before `*/`, so the note would land on the end of a
                # sentence.
                lead = "" if cut == 0 or cand[cut - 1] == "\n" else "\n"
                cand = cand[:cut] + lead + note + cand[cut:]
        out.append(cand)
    return out or [body], []


def probe_member(text, unit, member, body, decls):
    """(ok, candidate text, message) for one spelling of one member's body."""
    try:
        cand = add_body(text, member, body, decls)
    except ValueError as e:
        return False, None, str(e)
    # A stale forward declaration of this very function, left by another
    # family's body that only took its address, is `conflicting types'
    # once the definition arrives. Same defect dngland fixes for the
    # representative; it reaches the clones through exactly the same door.
    cand = fix_forward_decl(cand, member)
    # EVERY FUNCTION THE UNIT DEFINES, in the one compile ovlverify
    # already does. Adding a definition can change what an earlier landed
    # body compiles against -- the reconciled prototype above is that --
    # and naming the others costs nothing but a comparison each.
    others = [f for f in defined_funcs(cand) if f != member]
    with tempfile.TemporaryDirectory(prefix="dngclone-") as td:
        probe = os.path.join(td, unit + ".c")
        open(probe, "w").write(cand)
        env = dict(os.environ, AZURE_SCRATCH=os.path.join(td, "sp"))
        r = subprocess.run(
            [sys.executable,
             os.path.join(PROJ, "tools", "decomp", "ovlverify.py"),
             probe, member] + others,
            capture_output=True, text=True, env=env, cwd=PROJ)
        rows = [l.strip() for l in r.stdout.splitlines()
                if re.match(r"^func_\S+: ", l)]
    first = rows[0] if rows else ""
    ok = (" 0 differing " in first
          and re.search(r"target (\d+) words, candidate \1 words", first)
          is not None)
    if ok:
        broke = [l for l in rows[1:] if " 0 differing " not in l]
        if broke:
            ok = False
            first = ("REGRESSED another landed function in this unit: "
                     + broke[0])
    # The WHOLE stderr, not a 200-character prefix. A truncated traceback
    # showed only `main()` and cost a diagnosis: the useful line of a
    # Python traceback is its LAST one.
    msg = first.strip() or (r.stderr.strip().replace("\n", " | ")
                            or "no output")
    return ok, (cand if ok else None), msg


def local_callee_sigs(rep_text, rep_body):
    """{sym: prototype} for bank-local callees the REP'S OWN UNIT DEFINES.

    A CLONE NEEDS DECLARATIONS THE REPRESENTATIVE NEVER DID. If a callee is
    defined ABOVE the representative in its own unit, no declaration is
    required there and `needed_decls` has nothing to carry. In the target unit
    the renamed callee may be defined BELOW the insertion point, or still be
    an INCLUDE_ASM stub, and the compile then fails with
    `func_s01_80024024' undeclared -- naming a symbol that exists, in a file
    nobody wrote by hand. Four members of func_b06_80174214's family failed
    that way on `func_b06_80173E94', which ovl_dng_b06.c defines earlier.

    The prototype comes from the definition, which is the authority, and is
    renamed with the rest of the body.
    """
    code = "".join(c for c, k in zip(rep_body, code_mask(rep_body)) if k)
    out = {}
    for sym in set(re.findall(r"\bfunc_[A-Za-z0-9]+_[0-9A-Fa-f]{8}\b", code)):
        if not TAGGED_LOCAL.match(sym):
            continue
        sig = def_signature(rep_text, sym)
        if sig:
            out[sym] = sig
    return out


def clone_unit(rep, rep_body, rep_decls, unit, members, dry_run,
               local_sigs=None):
    """Clone every member of one family that lives in ONE unit, in sequence.

    THE MEMBERS OF A FAMILY ARE NOT ONE PER UNIT. func_b01_801735D4's family
    has five members in ovl_dng_b01 alone. An earlier cut of this ran one
    thread per MEMBER, so those five read the same file, each spliced its own
    body into its own copy of the original text, and the last writer won --
    four landings silently lost per unit, with every one of them reporting
    `0 differing` because each was verified against a file that was correct
    when it was written and dead by the time it was on disk.

    So the thread pool is over UNITS and the members inside one are sequential,
    each verified against the text the previous one left.
    """
    path = os.path.join(PROJ, "csrc", unit + ".c")
    text = open(path).read()
    out = []
    for member in members:
        try:
            smap = symbol_map(rep, member)
        except ValueError as e:
            out.append((member, False, str(e)))
            continue
        body, _unmapped = rename(rep_body, smap)
        pairs = const_pairs(rep, member)
        if pairs:
            bodies, unsure = vary_consts(body, pairs)
            if unsure:
                out.append((member, False,
                            "per-copy immediate not placeable in the body: "
                            + "; ".join(unsure)))
                continue
        else:
            bodies = [body]
        want = list(rep_decls)
        # A callee the rep's unit defines above it (see local_callee_sigs):
        # add the renamed prototype unless THIS unit defines it too and the
        # definition comes first, which reconcile_decls then settles.
        for sym, sig in (local_sigs or {}).items():
            img = smap.get(sym)
            if img and img != member:
                want.append(sig)
        decls = reconcile_decls([rename(d, smap)[0] for d in want], text)
        # SEVERAL CANDIDATE SPELLINGS, and the verifier picks. vary_consts
        # returns one body per placement of an immediate that occurs more
        # often in the code than it differs in the listing; trying them in
        # turn is strictly better than refusing, and costs one compile each
        # only on the members that are actually ambiguous.
        ok = False
        for i, cand_body in enumerate(bodies):
            ok, cand, msg = probe_member(text, unit, member, cand_body, decls)
            if ok:
                text = cand
                if len(bodies) > 1:
                    msg += (f"  [spelling {i + 1} of {len(bodies)} for the "
                            f"per-copy immediate]")
                break
        if not ok and len(bodies) > 1:
            msg += (f"  [all {len(bodies)} placements of the per-copy "
                    f"immediate failed]")
        out.append((member, ok, msg))
    if not dry_run and any(ok for _, ok, _ in out):
        open(path, "w").write(text)
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("func", help="the LANDED representative")
    ap.add_argument("--src", help="unit file holding the landed body "
                                  "(default csrc/<unit>.c)")
    ap.add_argument("--dry-run", action="store_true",
                    help="verify every member, write nothing")
    ap.add_argument("--only", help="comma-separated member function names")
    ap.add_argument("--hold", default="",
                    help="comma-separated unit tags a LIVE AGENT holds; their "
                         "members are skipped so a clone cannot land work "
                         "someone is still doing")
    ap.add_argument("-j", type=int, default=8)
    args = ap.parse_args()

    rep = args.func
    src = args.src or unit_path(rep)
    rep_text = open(src).read()
    rep_body, _ = definition(rep_text, rep)
    if rep_body is None:
        raise SystemExit(f"{rep} is not defined in {src} -- land it first")
    rep_decls = needed_decls(decl_block(rep_text), rep_body)
    bad = placeholder_comment(rep_body)
    if bad:
        raise SystemExit(
            "%s's comment still says `%s' -- it is a placeholder, and cloning "
            "would copy it into every member. Re-sync the representative from "
            "the agent's FINAL copy first (see placeholder_comment)."
            % (rep, bad))
    local_sigs = local_callee_sigs(rep_text, rep_body)

    # `landed=True` BECAUSE THE REPRESENTATIVE IS ALREADY LANDED by the time
    # this runs -- dngland.py writes it into csrc/ and then calls this. The
    # default view hides landed functions, which is right for ranking work and
    # would hide the very function being cloned.
    fam = None
    for g in dngfam.families(landed=True).values():
        if any(n == rep for _, n, _ in g):
            fam = g
            break
    if fam is None:
        raise SystemExit(f"{rep}: no listing, so no family to clone into")
    # Members that are themselves already landed have no INCLUDE_ASM line to
    # splice over, so they are not work -- skip them rather than fail on them.
    still = dngfam.stubs()
    members = [n for _, n, _ in fam if n != rep and n in still]
    if args.only:
        want = set(args.only.split(","))
        members = [m for m in members if m in want]
    # A CLONE CAN LAND A FUNCTION A LIVE AGENT IS STILL WORKING ON. Batch 8
    # gave agent-h17 three ovl_dng_b28 stubs, and while it worked, cloning
    # another agent's family reached into b28 and landed all three. Nothing
    # was wrong -- h17's own bodies also measured 0 differing -- but it spent
    # 611 words of its run on work that was already done, and it had to tell
    # ME that rather than the other way round.
    if args.hold:
        held = {t for t in args.hold.split(",") if t}
        skipped = [m for m in members if unit_of(m).split("_")[-1] in held]
        members = [m for m in members if m not in skipped]
        if skipped:
            print("holding %d member(s) in a live agent's unit: %s"
                  % (len(skipped), ", ".join(skipped)))
    print(f"{rep}: {len(members)} other members, {fam[0][2]} words each")

    by_unit = {}
    for m in members:
        by_unit.setdefault(unit_of(m), []).append(m)
    good = bad = 0
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.j) as ex:
        futs = [ex.submit(clone_unit, rep, rep_body, rep_decls, u, ms,
                          args.dry_run, local_sigs)
                for u, ms in by_unit.items()]
        for f in concurrent.futures.as_completed(futs):
            for m, ok, msg in f.result():
                if ok:
                    good += 1
                else:
                    bad += 1
                    print(f"  FAIL {m}: {msg}")
    print(f"{good} verified{'' if args.dry_run else ' and written'}, {bad} failed, "
          f"{good * fam[0][2]} words")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
