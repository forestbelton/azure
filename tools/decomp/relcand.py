#!/usr/bin/env python3
"""relcand.py -- audit a CANDIDATE's relocated symbols against its listing,
WITHOUT a build. The check `ovlverify' provably cannot do, available to an
agent before anything is landed.

## WHY THIS EXISTS

`ovlverify' compares words with the relocated fields MASKED, so the IDENTITY
of a relocated symbol is invisible to it: a dress site naming the wrong
bank-local table measures 0 differing and the body is still wrong. Shown
deliberately twice -- `func_b31_801716F4' (batch 14) and `func_b14_80170E9C'
(batch 18, where spelling D_80174F10 as D_80174EE0 measured 349 words /
0 differing). Call targets are as invisible: `func_b38_80173F0C' calls `rand'
and, spelled as the unrelated `func_800A6D30()', still measured 122 / 0.

`relocheck.py' closes that gap over the LANDED corpus, but it reads
`build/csrc/UNIT.s' and so needs `make check' -- which agents must not run,
because it prunes the listings the twin tooling reads. So every agent has been
doing this audit BY HAND. This does it mechanically from the candidate alone.

Derived from agent-h76's `relsyms.py' (batch 20), which did the candidate half.

    python3 tools/decomp/relcand.py CANDIDATE.c FUNC [FUNC ...]

## WHAT IT COMPARES, AND THE THREE TRAPS IT HAD TO GET PAST

  DATA   `D_8xxxxxxx' symbols, as a SET and deliberately not a count. Four
         resident symbols share the 0x8008 page on this block, so a
         cross-jumped `lui' serves more than one `%lo' and splat names it
         after whichever it reached -- a `%hi' tally is not a count of source
         references (batch 18). Identity is what catches a wrong table and
         identity is sound.
  CALLS  `jal' targets as a multiset, canonicalised through
         `splat/common_syms.txt' so the listing's address form and a real name
         are one symbol (`func_80065420' and `RotTransPers').

TRAP 1 -- `-G0' IS NOT OPTIONAL. Without it the assembler expands a sized
`.extern' into the `lui $at' form and the relocation count changes: h76's
first run reported 4 relocations against `D_800814A0' where the target has 3,
which reads exactly like a real defect. Same trap `ovlverify's docstring opens
with, through a different door.

TRAP 2 -- AN EXTERNAL CALL'S DISASSEMBLY ANNOTATION IS A LIE. R_MIPS_26
against a real symbol leaves the instruction field 0, so `objdump -d' prints
`jal 0 <X>' where X is whatever symbol sits at `.text+0'. Taking the
annotation makes every external call in a unit read as that unit's first
function.

TRAP 3 -- AN INTRA-UNIT CALL'S RELOCATION IS A LIE. It is REL-format against
the section with the addend in the instruction field, so `objdump -r' prints a
bare `.text' -- 103 of them in one unit -- and cannot name the target, while
the annotation there IS correct.

So calls are read from BOTH streams, paired per instruction: the relocation's
symbol when it names one, the annotation when the relocation says `.text'.
"""
import collections
import io
import os
import re
import subprocess
import sys

PROJ = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(PROJ, "tools", "decomp"))
import asmlib  # noqa: E402

DATA = re.compile(r"^(D_(?:[a-z0-9]+_)?8[0-9A-Fa-f]{7})$")
DATA_ANY = re.compile(r"\b(D_(?:[a-z0-9]+_)?8[0-9A-Fa-f]{7})\b")
CALL = re.compile(r"\bjal\s+([A-Za-z_][A-Za-z0-9_]*)")
INSN = re.compile(r"^\s*([0-9a-f]+):\t[0-9a-f ]+\t(\S+)\s*(.*)$")
ANN = re.compile(r"<([A-Za-z_][A-Za-z0-9_]*)(?:\+0x[0-9a-f]+)?>")
FN = re.compile(r"^[0-9a-f]+ <([^>]+)>:")
# The lowest address any LOADED image covers.  This was 0x8003C9F8 -- the
# resident CODE start -- until agent-h95 hand-audited an s-unit in batch 23 and
# found this filter was excluding four of its five data symbols.  The s-objects
# load at vram 0x80024000, so their BANK-LOCAL data lives at 0x80026xxx and
# 0x80027xxx, BELOW the old threshold: every s-unit's table references were
# being silently skipped by both this tool and relocheck.py, which is exactly
# the wrong-bank-table defect they exist to catch.
#
# The exclusion itself is still right -- an address in no loaded image is one
# splat INFERRED from a lui/lhu pair, and the C that matches is usually a bare
# constant -- but the boundary is the lowest loaded VRAM, not the resident code
# start.  The original false alarm that motivated the filter (D_80013714 at
# 0x80013714) is still below this and still excluded.
LOW = 0x80024000


def in_image(name):
    try:
        return int(name[-8:], 16) >= LOW
    except ValueError:
        return True


def name_map():
    out = {}
    p = os.path.join(PROJ, "splat", "common_syms.txt")
    if not os.path.exists(p):
        return out
    for line in io.open(p, encoding="utf-8", errors="replace"):
        m = re.match(r"\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(0x[0-9a-fA-F]+)\s*;", line)
        if m:
            out["func_%s" % m.group(2)[2:].lower()] = m.group(1)
    return out


NAMES = name_map()


def canon(s):
    low = s.lower()
    if low in NAMES:
        return NAMES[low]
    return low if re.match(r"^func_8[0-9a-f]{7}$", low) else s


def candidate(src):
    """{func: (set(data), Counter(calls))} from the compiled candidate."""
    unit = os.path.splitext(os.path.basename(src))[0]
    asmlib.build(src, gasG=0, cflags=("-G0",), unit=unit)
    obj = os.path.join(asmlib.procdir(), unit + ".o")
    txt = subprocess.run(
        ["mips64-linux-gnu-objdump", "-d", "-z", "-r", "-j", ".text", obj],
        capture_output=True, text=True).stdout
    out, cur, pending = {}, None, None
    for ln in txt.split("\n"):
        m = FN.match(ln)
        if m:
            cur, pending = m.group(1), None
            out.setdefault(cur, (set(), collections.Counter()))
            continue
        if cur is None:
            continue
        i = INSN.match(ln)
        if i:
            ann = ANN.search(i.group(3) or "")
            pending = (i.group(2), ann.group(1) if ann else None)
            continue
        if "R_MIPS" not in ln:
            continue
        q = ln.split()
        kind, sym = q[-2], q[-1].split("+")[0]
        if kind.endswith("26"):
            # TRAP 4: a `j' to a label inside the same function is ALSO
            # R_MIPS_26 against `.text'. Only a `jal' is a call -- counting
            # the jumps put 15 phantom `.text' entries on one body.
            if pending and pending[0] == "jal":
                out[cur][1][canon(sym if sym != ".text"
                                  else (pending[1] or sym))] += 1
            pending = None
        elif kind.endswith("HI16") and DATA.match(sym) and in_image(sym):
            out[cur][0].add(sym)
    return out


def listing(func):
    m = re.match(r"^func_([A-Za-z][A-Za-z0-9]*)_", func)
    if m is None:          # not a function name at all (a stray flag, say)
        return None
    p = os.path.join(PROJ, "asm", "nonmatchings",
                     "ovl_dng_" + m.group(1), func + ".s")
    if not os.path.exists(p):
        return None
    t = open(p, errors="replace").read()
    # a jump table BELONGS to the function; it is not a reference to one
    t = re.sub(r"\.section \.rodata.*?\.section \.text", "", t, flags=re.S)
    return ({x.group(1) for x in DATA_ANY.finditer(t) if in_image(x.group(1))},
            collections.Counter(canon(x.group(1)) for x in CALL.finditer(t)))


def main():
    if len(sys.argv) < 3:
        raise SystemExit("usage: relcand.py CANDIDATE.c FUNC [FUNC ...]")
    src, funcs = sys.argv[1], sys.argv[2:]
    cand = candidate(src)
    bad = 0
    for f in funcs:
        want = listing(f)
        if want is None:
            print("%-26s NO LISTING (already landed? use relocheck after a build)" % f)
            continue
        if f not in cand:
            print("%-26s NOT IN THE CANDIDATE OBJECT" % f)
            bad += 1
            continue
        gd, gc = cand[f]
        wd, wc = want
        md, xd, mc, xc = wd - gd, gd - wd, wc - gc, gc - wc
        if md or xd or mc or xc:
            bad += 1
            print("%s:" % f)
            if md:
                print("    DATA in the listing, not in your code: %s" % ", ".join(sorted(md)))
            if xd:
                print("    DATA in your code, not in the listing: %s" % ", ".join(sorted(xd)))
            if mc:
                print("    CALLS the listing makes, yours does not: %s" % ", ".join(sorted(mc.elements())))
            if xc:
                print("    CALLS your code makes, the listing does not: %s" % ", ".join(sorted(xc.elements())))
        else:
            print("%-26s ok  (%d data symbols, %d calls)" % (f, len(gd), sum(gc.values())))
    print("\n%d function(s) checked, %d with a mismatch" % (len(funcs), bad))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
