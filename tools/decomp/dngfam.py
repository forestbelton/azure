#!/usr/bin/env python3
"""Which of DUNGEON.BIN's 2575 carved stubs are COPIES of each other.

The 116 objects carved out of DUNGEON.BIN on 2026-11-22 are not 116 separate
programs. The 47 banked room scripts share a common library, and that library
is statically linked into every object that uses it -- so the same function
appears, relinked, in up to 216 of them. Measured here: **1199 of 2575 stubs
are exact copies of a sibling, 93,302 of 327,714 words.** 1376 distinct
functions cover the whole block.

That makes the unit of work a FAMILY, not a function. Land the representative
once, then replicate the body into every member with `dngclone.py` -- which is
sound exactly because the copies differ only in the relocation operands this
tool normalises away.

## What counts as the same function

Two bodies are in one family when their instruction streams are identical
after replacing

* every symbolic operand -- `.L<tag>_<vram>`, `func_<tag>_<vram>`, `D_...`,
  `jtbl_...` -- with `S`, and
* every hex literal with `N`.

Both halves are needed and neither is enough on its own.

ONLY THE SYMBOL CLASS IS NORMALISED, NOT EVERY IDENTIFIER. Registers stay.
An earlier cut of this matched on OPCODES alone and reported 1052 redundant
copies across families whose registers differed -- different source compiled
to the same control flow. The opposite error is just as easy: a first attempt
at the symbol regex anchored on `\b`, which never matches before the `.` of
`.Lb01_80171C90`, so every branch label read as a difference and a family of
35 identical 466-word bodies came out as 35 distinct functions. The two
mistakes bracket the right answer, and the right answer is this one: 201
families, verified by diffing members instruction for instruction (see
--diff, which prints what actually differs between two members).

Hex literals are normalised because a relinked copy's `lui`/`addiu` pair
carries a different address. That does mean two functions differing ONLY in a
constant land in one family -- an immediate `li $a0, 3` against `li $a0, 4`.
`dngclone.py` re-verifies every member it writes against that member's own
listing, so such a pair costs a failed verify, not a wrong landing.

Usage:
    python3 tools/decomp/dngfam.py                 # families by word yield
    python3 tools/decomp/dngfam.py --all           # every family, including 1
    python3 tools/decomp/dngfam.py FUNC            # FUNC's family, listed
    python3 tools/decomp/dngfam.py --diff A B      # why two bodies differ
    python3 tools/decomp/dngfam.py --json PATH     # the whole map
"""
import argparse
import collections
import glob
import hashlib
import json
import os
import re
import sys

PROJ = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

ROW = re.compile(r"\s*/\* \w+ \w+ [0-9A-F]{8} \*/\s+(.*)")
# No \b in front: a branch label starts with '.', which is not a word
# character, so \b would never fire there. See the docstring.
SYM = re.compile(r"(?:\.L|func_|D_|jtbl_)[A-Za-z0-9_]*[0-9A-Fa-f]{8}")
HEX = re.compile(r"-?0x[0-9a-fA-F]+")


def sig_insn(text):
    return re.sub(r"\s+", " ", HEX.sub("N", SYM.sub("S", text))).strip()


LABEL = re.compile(r"^\s*(\.L[A-Za-z0-9_]+):")


def body_labels(path, name):
    """([instruction text], {label: instruction index}) for `name`.

    A carved listing can hold several symbols -- b31f and lo20 carry rodata
    islands between functions -- so the glabel/endlabel scan is not optional.
    """
    out, labs, inside = [], {}, False
    for ln in open(path):
        if ln.startswith("glabel "):
            inside = ln.split()[1].strip() == name
            continue
        if ln.startswith("endlabel") and inside:
            break
        if not inside:
            continue
        lm = LABEL.match(ln)
        if lm:
            labs[lm.group(1)] = len(out)
            continue
        m = ROW.match(ln)
        if m:
            out.append(m.group(1).strip())
    return out, labs


def body(path, name):
    """[instruction text] for `name` out of its listing."""
    return body_labels(path, name)[0]


def targets(insns, labs):
    """[resolved branch target index, or None] per instruction.

    WHY THIS IS NOT OPTIONAL. `sig_insn` replaces a label with `S`, so two
    bodies whose branches go to DIFFERENT PLACES can compare equal
    instruction for instruction. On func_b14_80171410 that made `--diff`
    against func_b01_80171C34 report exactly one difference -- a `nop` where
    b01 materialises a zero -- and hid that the branch above it goes to a
    different label as well. The real difference was a brace: b01's `0x8000`
    gate wraps two arms where b14's wraps one. An agent told "one word apart"
    spent its first compiles looking for a spelling.
    """
    out = []
    for t in insns:
        hit = None
        for lab in re.findall(r"\.L[A-Za-z0-9_]+", t):
            if lab in labs:
                hit = labs[lab]
                break
        out.append(hit)
    return out


def listing_path(func):
    """The carved listing holding `func`, from its `func_<tag>_<vram>` name."""
    m = re.match(r"^func_([A-Za-z][A-Za-z0-9]*)_[0-9A-Fa-f]{8}$", func)
    if not m:
        raise SystemExit(f"{func} is not a carved DUNGEON.BIN name")
    return os.path.join(PROJ, "asm", "nonmatchings",
                        "ovl_dng_" + m.group(1), func + ".s")


def stubs():
    """Every carved function still carried by INCLUDE_ASM, from csrc/.

    A LISTING IS NOT EVIDENCE THAT A FUNCTION IS STILL A STUB. splat is
    documented to prune the listing of a landed function, and under this
    block's spec it does not: after batch 1 there were 2577 listings against
    2015 stubs, so a ranking taken from listings alone offered work that was
    already done and `dngclone` would have gone looking for an INCLUDE_ASM
    line that is not there. csrc is the authority.
    """
    out = set()
    for f in glob.glob(os.path.join(PROJ, "csrc", "ovl_dng_*.c")):
        out |= set(re.findall(r'INCLUDE_ASM\("[^"]*",\s*(func_\w+)\)',
                              open(f).read()))
    return out


def families(landed=False):
    """{digest: [(unit, func, words)]} over every carved stub listing.

    Landed functions are left out unless `landed` -- see stubs().
    """
    keep = None if landed else stubs()
    H = collections.defaultdict(list)
    for d in sorted(glob.glob(os.path.join(PROJ, "asm", "nonmatchings",
                                           "ovl_dng_*"))):
        unit = os.path.basename(d)
        for f in sorted(glob.glob(os.path.join(d, "*.s"))):
            name = os.path.basename(f)[:-2]
            if keep is not None and name not in keep:
                continue
            b = body(f, name)
            if not b:
                # b31f/lo20 rodata islands leave a symbol with no
                # instructions under it. Nothing to decompile and nothing
                # to compare; see splat/dungeon.bin.yaml.
                continue
            key = hashlib.md5("|".join(sig_insn(x) for x in b).encode()).hexdigest()
            H[key].append((unit, name, len(b)))
    return H


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("func", nargs="?")
    ap.add_argument("--all", action="store_true")
    ap.add_argument("--diff", nargs=2, metavar=("A", "B"))
    ap.add_argument("--json", metavar="PATH")
    ap.add_argument("-n", type=int, default=30)
    ap.add_argument("--landed", action="store_true",
                    help="include functions that are no longer INCLUDE_ASM")
    args = ap.parse_args()

    if args.diff:
        a, b = args.diff
        A, la = body_labels(listing_path(a), a)
        B, lb = body_labels(listing_path(b), b)
        ta, tb = targets(A, la), targets(B, lb)
        print(f"{a}: {len(A)} words    {b}: {len(B)} words")
        n = jumps = 0
        for i in range(max(len(A), len(B))):
            x = A[i] if i < len(A) else "--"
            y = B[i] if i < len(B) else "--"
            if sig_insn(x) != sig_insn(y):
                n += 1
                print(f"  {i:4d} {x:42s} | {y}")
            elif (i < len(ta) and i < len(tb) and ta[i] != tb[i]):
                jumps += 1
                print(f"  {i:4d} {x:42s} | {y}"
                      f"   <<< same instruction, TARGET {ta[i]} vs {tb[i]}")
        print(f"{n} instructions differ after normalisation, "
              f"{jumps} more branch to a different place")
        if n == 0 and jumps == 0:
            print("SAME FAMILY")
        elif n == 0:
            print("NOT the same function: the streams match but the control "
                  "flow does not -- look for a structural difference (a brace, "
                  "a gate wrapping one arm instead of two), not a spelling")
        else:
            print("different functions")
        return

    H = families(args.landed)
    if args.json:
        json.dump({k: v for k, v in H.items()}, open(args.json, "w"))
        print(f"wrote {args.json}: {len(H)} signatures")
        return

    if args.func:
        for g in H.values():
            if any(n == args.func for _, n, _ in g):
                print(f"{len(g)} members, {g[0][2]} words each, "
                      f"{len(g) * g[0][2]} words total")
                for u, n, w in g:
                    print(f"  {n:28s} {u}")
                return
        raise SystemExit(f"{args.func}: no listing (landed, or not carved)")

    groups = sorted(H.values(), key=lambda v: -(len(v) * v[0][2]))
    if not args.all:
        groups = [g for g in groups if len(g) > 1]
    tot = sum(v[0][2] * len(v) for v in H.values())
    red = sum((len(v) - 1) * v[0][2] for v in H.values())
    print(f"{sum(len(v) for v in H.values())} stubs, {len(H)} distinct "
          f"functions, {sum(1 for v in H.values() if len(v) > 1)} families")
    print(f"{tot} words, {red} of them in copies "
          f"({100.0 * red / tot:.1f}% recoverable by replication)")
    print()
    print(f"{'copies':>6s} {'words':>5s} {'yield':>7s}  representative")
    for g in groups[:args.n]:
        print(f"{len(g):6d} {g[0][2]:5d} {len(g) * g[0][2]:7d}  {g[0][1]}")


if __name__ == "__main__":
    main()
