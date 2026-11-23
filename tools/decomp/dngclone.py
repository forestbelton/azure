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


def definition(src_text, func):
    """(comment + definition, span) for `func` in a unit's source."""
    m = re.search(r"(?m)^[A-Za-z_][\w \*\t]*?\b%s\s*\(" % re.escape(func),
                  src_text)
    if not m:
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


def decl_block(src_text):
    """The unit's declaration block: everything between the include and the
    first definition or stub, as individual statements."""
    m = re.search(r'#include "common\.h"\n', src_text)
    start = m.end() if m else 0
    end = len(src_text)
    for pat in (r"(?m)^INCLUDE_ASM\(", r"(?m)^[A-Za-z_][\w \*\t]*\w+\s*\([^;]*\)\s*\n\{"):
        mm = re.search(pat, src_text[start:])
        if mm:
            end = min(end, start + mm.start())
    chunk = src_text[start:end]
    return [s.strip() for s in chunk.split("\n") if s.strip()]


# The identifier a declaration declares: the first one followed by `(`, `[`
# or `;`. Covers `s16 func_X(void *, s16);`, `extern u8 D_Y[];`,
# `extern u16 D_Z;` and the alias form `u8 *f_r(u8 *) __asm__("f");`.
DECLARED = re.compile(r"\b([A-Za-z_]\w*)\s*(?=\(|\[|;)")


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
    the body refers to it by a name this cannot see.
    """
    out = []
    for d in decls:
        if d.startswith(("typedef", "struct", "union", "enum", "#")):
            out.append(d)
            continue
        m = DECLARED.search(d)
        if m and re.search(r"\b%s\b" % re.escape(m.group(1)), body):
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


def add_body(text, member, body, decls):
    """`text` with `member`'s stub replaced by `body` and `decls` ensured."""
    have = set(decl_block(text))
    add = [d for d in decls if d not in have]
    if add:
        m = re.search(r'#include "common\.h"\n', text)
        at = m.end() if m else 0
        text = text[:at] + "\n" + "\n".join(add) + "\n" + text[at:]
    return splice(text, member, body)


def clone_unit(rep, rep_body, rep_decls, unit, members, dry_run):
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
        decls = [rename(d, smap)[0] for d in rep_decls]
        try:
            cand = add_body(text, member, body, decls)
        except ValueError as e:
            out.append((member, False, str(e)))
            continue
        with tempfile.TemporaryDirectory(prefix="dngclone-") as td:
            probe = os.path.join(td, unit + ".c")
            open(probe, "w").write(cand)
            env = dict(os.environ, AZURE_SCRATCH=os.path.join(td, "sp"))
            r = subprocess.run(
                [sys.executable,
                 os.path.join(PROJ, "tools", "decomp", "ovlverify.py"),
                 probe, member],
                capture_output=True, text=True, env=env, cwd=PROJ)
            first = (r.stdout.splitlines() or [""])[0]
        ok = " 0 differing " in first and re.search(
            r"target (\d+) words, candidate \1 words", first) is not None
        if ok:
            text = cand
        # The WHOLE stderr, not a 200-character prefix. A truncated traceback
        # showed only `main()` and cost a diagnosis: the useful line of a
        # Python traceback is its LAST one.
        out.append((member, ok,
                    first.strip() or (r.stderr.strip().replace("\n", " | ")
                                      or "no output")))
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
    ap.add_argument("-j", type=int, default=8)
    args = ap.parse_args()

    rep = args.func
    src = args.src or unit_path(rep)
    rep_text = open(src).read()
    rep_body, _ = definition(rep_text, rep)
    if rep_body is None:
        raise SystemExit(f"{rep} is not defined in {src} -- land it first")
    rep_decls = needed_decls(decl_block(rep_text), rep_body)

    fam = None
    for g in dngfam.families().values():
        if any(n == rep for _, n, _ in g):
            fam = g
            break
    if fam is None:
        raise SystemExit(f"{rep}: no listing, so no family to clone into")
    members = [n for _, n, _ in fam if n != rep]
    if args.only:
        want = set(args.only.split(","))
        members = [m for m in members if m in want]
    print(f"{rep}: {len(members)} other members, {fam[0][2]} words each")

    by_unit = {}
    for m in members:
        by_unit.setdefault(unit_of(m), []).append(m)
    good = bad = 0
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.j) as ex:
        futs = [ex.submit(clone_unit, rep, rep_body, rep_decls, u, ms,
                          args.dry_run)
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
