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
CSYM = re.compile(r"\b(?:func|D|jtbl)_[A-Za-z][A-Za-z0-9]*_[0-9A-Fa-f]{8}\b")
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

    A symbol the map does not mention is left alone ON PURPOSE: that is how a
    reference OUT of the object -- `func_800ABDBC` in the resident overlay --
    survives, since it is the same symbol in every copy and so pairs with
    itself above. A carved symbol that is genuinely unmapped would be a
    reference the listings do not witness, so it is reported.
    """
    missing = set()

    def sub(m):
        s = m.group(0)
        if s in smap:
            return smap[s]
        missing.add(s)
        return s
    return CSYM.sub(sub, text), missing


def definition(src_text, func):
    """(comment + definition, span) for `func` in a unit's source."""
    m = re.search(r"(?m)^[A-Za-z_][\w \*\t]*?\b%s\s*\(" % re.escape(func),
                  src_text)
    if not m:
        return None, None
    start = m.start()
    # Pull in an immediately preceding comment block, which carries the
    # landing's word count and the spelling fact that made it land.
    head = src_text[:start].rstrip("\n")
    cut = head.rfind("\n\n")
    lead = head[cut + 2:] if cut != -1 else head
    if lead.lstrip().startswith(("/*", "//")):
        start = (cut + 2) if cut != -1 else 0
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


def splice(unit_text, func, body):
    """Replace `func`'s INCLUDE_ASM line with `body`."""
    pat = re.compile(r'(?m)^INCLUDE_ASM\("[^"]*",\s*%s\);\s*$' % re.escape(func))
    if not pat.search(unit_text):
        raise ValueError(f"{func}: no INCLUDE_ASM stub in its unit")
    return pat.sub(lambda _: body.replace("\\", "\\\\"), unit_text, count=1)


def clone_one(rep, rep_body, rep_decls, member, dry_run):
    """-> (member, ok, message). Writes the member's unit only on 0 differing."""
    try:
        smap = symbol_map(rep, member)
    except ValueError as e:
        return member, False, str(e)
    body, missing = rename(rep_body, smap)
    if missing:
        return member, False, f"unmapped symbols: {', '.join(sorted(missing))}"
    decls = []
    for d in rep_decls:
        r, _ = rename(d, smap)
        decls.append(r)

    path = unit_path(member)
    text = open(path).read()
    have = set(decl_block(text))
    add = [d for d in decls if d not in have]
    new = text
    if add:
        m = re.search(r'#include "common\.h"\n', new)
        at = m.end() if m else 0
        new = new[:at] + "\n" + "\n".join(add) + "\n" + new[at:]
    try:
        new = splice(new, member, body)
    except ValueError as e:
        return member, False, str(e)

    with tempfile.TemporaryDirectory(prefix="dngclone-") as td:
        probe = os.path.join(td, unit_of(member) + ".c")
        open(probe, "w").write(new)
        env = dict(os.environ, AZURE_SCRATCH=os.path.join(td, "sp"))
        r = subprocess.run(
            [sys.executable, os.path.join(PROJ, "tools", "decomp", "ovlverify.py"),
             probe, member],
            capture_output=True, text=True, env=env, cwd=PROJ)
        first = (r.stdout.splitlines() or [""])[0]
    ok = " 0 differing " in first and re.search(
        r"target (\d+) words, candidate \1 words", first) is not None
    if ok and not dry_run:
        open(path, "w").write(new)
    return member, ok, first.strip() or (r.stderr.strip()[:200] or "no output")


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
    rep_decls = decl_block(rep_text)

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

    good = bad = 0
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.j) as ex:
        futs = [ex.submit(clone_one, rep, rep_body, rep_decls, m, args.dry_run)
                for m in members]
        for f in concurrent.futures.as_completed(futs):
            m, ok, msg = f.result()
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
