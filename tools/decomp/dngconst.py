#!/usr/bin/env python3
"""dngconst -- find a stub's donor by SHARED LITERAL CONSTANTS.

A CONSTANT IN A STATEMENT IS AS DISTINCTIVE AS A CALLEE, AND THE RATIO TOOLS
ARE BLIND TO IT.  That is not my observation: an agent clearing ovl_dng_s68
had a unit with NO landed bodies and one `dngtwin' lead at 0.742, and what
actually landed all five functions was ovl_dng_s12 -- the same five-function
object relinked, same addresses, same roles, 100% landed with six priced
facts.  It found s12 by intersecting `GetClut' with `0xFFF7F7F8' and
`0x80808' across csrc/ovl_dng_*.c by hand.

`dngtwin' scores instruction-sequence similarity and `dngcallee' scores shared
symbols.  Neither sees a literal, and a literal is often the most specific
thing a body contains: a colour word, a fade step, a magic divisor, a flag
mask.  Two bodies that share `0x606060' and `0xFFF7F7F8' are doing the same
job whatever their lengths.

    dngconst.py func_b24_80173A60          rank donors for one stub
    dngconst.py func_b24_80173A60 -n 20    more rows
    dngconst.py --unit b24                 every stub in a unit, best row each
    dngconst.py func_... --show            list the shared constants per row

SCORING is by RARITY, 1/n_bodies_with_it, and the weight has to be this
steep.  My first version used 1/log(1+n) and it was useless: that separates a
constant in 35 bodies from one in 576 by a factor of 1.8, so the top four rows
tied exactly and every one of them was carried by `0x1000' (493 bodies),
`0x2000' (575) and `0x7FFF' (311) -- masks that say nothing about what a body
does.  With 1/n a constant in two bodies scores 0.5 and one in 500 scores
0.002, so only genuinely distinctive literals can move a row, which is the
whole point.  The rarest shared constant is printed beside each row because
it, not the sum, is usually what makes a donor worth opening.

WHEN IT WORKS AND WHEN IT DOES NOT, measured rather than assumed.  I validated
it against a known-correct answer before trusting it: `func_b22_80170F94' is a
162-word stub whose family contains the LANDED `func_b42_80170B48', so that
sibling is the right donor by construction.  **This tool ranks it 131st of
1,323 rows.**  Its listing holds only four constants above 0x100 and the real
sibling shares two, both of them in hundreds of bodies.

So the constant route works only when the target has a RARE literal, and it
cannot be used as a general donor finder.  The run that motivated it had
`0x606060', `0xFFF7F7F8' and `0x80808' -- a colour word, a fade mask and a
fade step, each in a handful of bodies.  A state machine full of `0x1000' and
`0x7FFF' has nothing to offer.

**SO BE CLEAR ABOUT THE QUESTION THIS ANSWERS.**  It answers *which landed
bodies share my rare literals*, which is well defined and correct.  It does
NOT answer *who is my donor*, and the validation above is the proof: the
target there HAS a rare literal (`0x1001', in 5 bodies), the verdict would
have read "usable", and the correct donor still ranked 131st because it shares
none of the rare ones.  A rare literal in the TARGET does not mean the donor
shares it.

Before any row the tool prints the rarest literals and says which of the two
questions the rows are answering, and when the best on offer is a common mask
it says every row below is noise and names the tools to use instead.  A ranked
list of noise is worse than no list, and the first version of this printed
one.

WHAT IT CANNOT SEE EVEN WHEN USABLE: a body whose constants were folded away,
a body that shares a SHAPE but no literals, and the direction of any
similarity -- a high row means "these two were written by someone doing the
same thing", not "copy this".  Read the donor before carrying anything, and
price every fact you carry on YOUR body.
"""

import argparse
import collections
import glob
import math
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dngclone  # noqa: E402
import dngfam  # noqa: E402

PROJ = dngfam.PROJ
DEFN = re.compile(r"^[A-Za-z_][A-Za-z0-9_ \*]*\b(func_[0-9A-Za-z_]+)\s*\([^;]*\)\s*$")
STUB = re.compile(r"INCLUDE_ASM\([^,]+,\s*(func_[0-9A-Za-z_]+)\s*\)")
HDR = re.compile(r"nonmatching (func_[0-9A-Za-z_]+), (0x[0-9A-Fa-f]+)")
PRE = re.compile(r"^\s*/\* [0-9A-F]+ [0-9A-F]+ [0-9A-F]+ \*/\s{1,2}")

# Below this, a constant is structural noise (small offsets, 0, 1, bit flags
# every body has) rather than a fingerprint.
FLOOR = 0x100

# A constant in more bodies than this cannot discriminate: at 493 bodies
# `0x1000' is a mask every state machine carries.  The validation case's best
# literal was in 5 bodies and its true donor's shared pair were in 493, which
# is where this line comes from.
RARE_ENOUGH = 40


def _norm(v):
    """A 32-bit constant, sign-folded so -0x18 and 0xFFFFFFE8 are one key."""
    v &= 0xFFFFFFFF
    return v - 0x100000000 if v >= 0x80000000 else v


def source_consts(text):
    """Literal constants >= FLOOR in a body's SOURCE text."""
    out = set()
    for m in re.finditer(r"0[xX]([0-9A-Fa-f]+)", text):
        v = int(m.group(1), 16)
        if abs(_norm(v)) >= FLOOR:
            out.add(_norm(v))
    for m in re.finditer(r"(?<![\w.])(\d{3,})(?![\w.])", text):
        v = int(m.group(1))
        if v >= FLOOR:
            out.add(_norm(v))
    return out


def listing_consts(text):
    """Constants in a LISTING, with lui/ori and lui/addiu pairs rebuilt.

    A source `0x606060' appears in asm as `lui 0x60' then `ori 0x6060', so
    reading immediates alone would never match it.  Rebuilding the pair is
    most of this tool's value on dress and colour code.
    """
    ins = []
    for line in text.split("\n"):
        if PRE.match(line):
            ins.append(PRE.sub("", line).rstrip())
    out = set()
    for i, s in enumerate(ins):
        m = re.match(r"lui\s+\$(\w+),\s*(?:0x)?([0-9A-Fa-f]+)\s*$", s)
        if m:
            hi = int(m.group(2), 16) << 16
            reg = m.group(1)
            paired = False
            for t in ins[i + 1:i + 4]:
                mo = re.match(r"ori\s+\$\w+,\s*\$%s,\s*(?:0x)?([0-9A-Fa-f]+)" % reg, t)
                if mo:
                    out.add(_norm(hi | int(mo.group(1), 16)))
                    paired = True
                    break
                ma = re.match(r"addiu\s+\$\w+,\s*\$%s,\s*(?:0x)?(-?[0-9A-Fa-f]+)" % reg, t)
                if ma:
                    lo = int(ma.group(1), 16)
                    if lo >= 0x8000:
                        lo -= 0x10000
                    out.add(_norm(hi + lo))
                    paired = True
                    break
            if not paired and abs(_norm(hi)) >= FLOOR:
                out.add(_norm(hi))
            continue
        for mo in re.finditer(r",\s*(-?)(?:0x)?([0-9A-Fa-f]{2,})\s*(?:$|\()", s):
            try:
                v = int(mo.group(2), 16)
            except ValueError:
                continue
            if mo.group(1):
                v = -v
            if abs(_norm(v)) >= FLOOR:
                out.add(_norm(v))
    return out


def landed_bodies():
    """-> [(unit, func, constset)] for every LANDED body in every csrc unit.

    Comments MUST be stripped before attributing a constant: a landed body's
    comment sits between the previous closing brace and its own definition
    line, and these comments are dense with hex figures from measured fact
    tables -- so a naive walk credits every one of them to the PRECEDING
    function.  dngcallee and dngclone both document this trap; `code_mask' is
    the one parser for it.
    """
    out = []
    for path in sorted(glob.glob(os.path.join(PROJ, "csrc", "*.c"))):
        unit = os.path.basename(path)[:-2]
        raw = open(path, errors="replace").read()
        stubs = set(STUB.findall(raw))
        txt = "".join(c for c, k in zip(raw, dngclone.code_mask(raw)) if k)
        cur, acc = None, []
        for ln in txt.split("\n"):
            m = DEFN.match(ln.rstrip())
            if m and m.group(1) not in stubs:
                if cur:
                    out.append((unit, cur, source_consts("\n".join(acc))))
                cur, acc = m.group(1), []
                continue
            if cur:
                acc.append(ln)
        if cur:
            out.append((unit, cur, source_consts("\n".join(acc))))
    return out


def stub_listing(func):
    for p in glob.glob(os.path.join(PROJ, "asm/nonmatchings/*/%s.s" % func)):
        return open(p, errors="replace").read()
    return None


def rank(func, pool, idf, limit, show):
    text = stub_listing(func)
    if text is None:
        print("%s: no listing (landed already, or not a stub)" % func)
        return
    want = listing_consts(text)
    if not want:
        print("%s: no constant above 0x%X in its listing -- nothing to match on"
              % (func, FLOOR))
        return
    rows = []
    for unit, fn, cs in pool:
        if fn == func:
            continue
        shared = want & cs
        if shared:
            rows.append((sum(idf[c] for c in shared), len(shared), fn, unit, shared))
    rows.sort(reverse=True)
    # THE VERDICT COMES FIRST, because a ranked list of noise is worse than no
    # list.  Validated: a 162-word stub with four common constants ranks its
    # own known-correct family sibling 131st of 1,323, so without this line
    # the tool actively misleads.
    present = [c for c in want if c in idf_n]
    rarest_n = min((idf_n[c] for c in present), default=0)
    rare = sorted(present, key=lambda c: idf_n[c])[:4]
    print("%s: %d constant(s) in its listing; rarest %s"
          % (func, len(want),
             ", ".join("%s in %d" % (hex(c & 0xFFFFFFFF) if c >= 0 else hex(c),
                                     idf_n[c]) for c in rare) or "none in the pool"))
    if not (0 < rarest_n <= RARE_ENOUGH):
        print("    NOTHING DISTINCTIVE: the rarest literal here is in %d bodies, so"
              % rarest_n)
        print("    every row below is noise. Use dngtwin, dngcallee or xsibs.")
    else:
        print("    These rows answer `which landed bodies share my rare literals',")
        print("    NOT `who is my donor'. On the one case with a ground truth the")
        print("    correct donor ranked 131st -- so a row here is a lead to read,")
        print("    and the donor may not be in the list at all.")
    if not rows:
        return
    for sc, n, fn, unit, shared in rows[:limit]:
        rarest = min(shared, key=lambda c: idf_n[c])
        print("    %6.3f  %-28s %-26s %2d shared, rarest %s in %d body(s)"
              % (sc, fn, unit, n,
                 hex(rarest & 0xFFFFFFFF) if rarest >= 0 else hex(rarest),
                 idf_n[rarest]))
        if show:
            pretty = sorted(shared, key=lambda c: -idf[c])
            print("            " + "  ".join(
                "%s(%d)" % (hex(c & 0xFFFFFFFF) if c >= 0 else hex(c), idf_n[c])
                for c in pretty[:10]))


if __name__ == "__main__":
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("func", nargs="?", default="")
    ap.add_argument("--unit", default="", help="every stub in this unit tag")
    ap.add_argument("-n", type=int, default=10, help="rows per stub")
    ap.add_argument("--show", action="store_true", help="list the shared constants")
    a = ap.parse_args()

    pool = landed_bodies()
    idf_n = collections.Counter()
    for _u, _f, cs in pool:
        for c in cs:
            idf_n[c] += 1
    idf = {c: 1.0 / n for c, n in idf_n.items()}
    print("pool: %d landed body(s), %d distinct constant(s)" % (len(pool), len(idf_n)))

    if a.unit:
        tag = a.unit[len("ovl_dng_"):] if a.unit.startswith("ovl_dng_") else a.unit
        todo = sorted(f for f in dngfam.stubs() if f.startswith("func_%s_" % tag))
        if not todo:
            raise SystemExit("dngconst: no stub in %s" % tag)
        for f in todo:
            rank(f, pool, idf, a.n, a.show)
    elif a.func:
        rank(a.func, pool, idf, a.n, a.show)
    else:
        raise SystemExit("dngconst: give a FUNC or --unit TAG")
