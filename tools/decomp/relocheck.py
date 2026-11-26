#!/usr/bin/env python3
"""relocheck.py -- does a LANDED body reference the SAME data symbols the
target does? `ovlverify' cannot tell you, and a clean landing can be wrong.

## THE GAP THIS CLOSES

`ovlverify' compares words with the relocated fields MASKED, because a
relocation's value is decided at link time and the candidate's cannot match
the ROM's until it is linked. That is the right call for code, and it means
**the identity of a relocated symbol is invisible to it**. A dress site that
loads the wrong bank-local table measures 0 differing and the body is still
wrong.

Found by agent-h46 in batch 14, on a function that landed at 365/365 and 0
differing: `func_b31_801716F4' has three facing-table dress sites, and the
0x80000 arm must load `D_8017555C' where the other two load `D_80175554'.
Writing `D_80175554' at all three ALSO measures 365 words and 0 aligned
differences. `ovlverify --raw' does not settle it either -- raw 87 differing
is exactly the relocation COUNT, and a wrong symbol of the same kind keeps
the count. The listing's relocations were the only evidence, and nothing
checked them.

## WHAT IT COMPARES

The multiset of RAM data symbols (`D_8xxxxxxx') each function references, taken
from the splat listing (the target) and from the built assembly maspsx
produced (what we actually compile). A multiset, not a sequence, because
sched and reorg move loads around freely and an order difference is not a
defect -- but a symbol that appears in one side and not the other is.

## CALL TARGETS ARE NOW CHECKED TOO, AND THE OLD REASON FOR SKIPPING THEM WAS WRONG

This file used to say call targets needed no checking because `ovlverify'
"already pins them through the `jal' opcode". **That is false, and agent-h66
measured it in batch 18:** `func_b38_80173F0C' calls `rand' (0x80069EF8), and
spelled as the COMPLETELY UNRELATED `func_800A6D30()' it still measures 122
words / 0 differing. The `jal' word is a relocation on both sides and compares
equal, so the callee's identity is exactly as invisible as a data symbol's.

The OTHER half of the old reason was real: the listing spells a call by
address (`func_80065420') where the source may use a real name from
`common_syms.txt' (`RotTransPers'), so a naive compare reports every renamed
callee. That is the same shape as the 71 false reports `jtbl_*' once produced.
The fix is the name map, which `splat/common_syms.txt' already is: both sides
are canonicalised through it before comparing, so `func_80065420' and
`RotTransPers' are one symbol.

Still deliberately NOT checked: `%lo' displacement forms, which the two sides
spell differently.

A unit must have been BUILT for its assembly to exist, so run this after
`make check', never instead of it.

Usage:
    python3 tools/decomp/relocheck.py                  # every built unit
    python3 tools/decomp/relocheck.py ovl_dng_b31      # one unit
"""
import collections
import io
import glob
import os
import re
import sys

PROJ = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
# Only RAM data symbols, `D_8xxxxxxx' and the bank-local `D_<tag>_8xxxxxxx'.
# Two kinds of name are deliberately out of scope because the two sides cannot
# spell them the same, and including them produced 71 false reports on the
# first run:
#
#   D_1F8000xx   the scratchpad. C writes it as an absolute constant
#                (`*(s32 *)0x1F80013C'), so the build has no symbol at all
#                where the listing names one. 69 of the 71.
#   below the resident code start, 0x8003c9f8. Same mechanism as the
#                scratchpad and the same reason: an address this low is not in
#                any loaded image, splat INFERS the name from the `lui'/`lhu'
#                pair, and the C that matches is often the bare constant. The
#                first real report this tool produced was exactly that --
#                func_b31f_8016D6F0 names `D_80013714' twice in its listing and
#                nothing in the build, because `(*(u16 *)0x80013714 & 8)' is
#                the ONLY spelling that lands (the declared symbol puts a
#                separate `%hi' where reorg lifts it into a delay slot the
#                target leaves empty: 234 words / 1 aligned difference).
#                The body is correct at 0 differing and the container is
#                byte-identical. A bank-local table -- which is what this tool
#                exists to check -- is always in the overlay slot or the
#                resident data above that boundary, so nothing is lost.
#   jtbl_*       a LANDED jump table is cc1psx's own `$L95'-style local label,
#                not the listing's `jtbl_' name, so a landed table owner
#                always looks like it dropped its table. The remaining 2.
#                THIS USED TO SAY "`ovlverify' pins a table's CONTENT through
#                the words", WHICH IS FALSE and was actively misleading.
#                ovlverify.py's own docstring says it compares ONE FUNCTION'S
#                words and "says nothing about the unit's .rodata" -- and a
#                jump table IS .rodata. Measured by an agent on 2026-11-25:
#                two arm orders of one 350-word switch BOTH read 350/350 and
#                0 aligned differences, while the emitted .rdata table's last
#                three entries came out reversed between them. So a table
#                owner can land at 0 differing with the WRONG ARM ORDER, and
#                `make containers' is the only check that sees it.
#                The cheap pre-check is a standalone `tools/cdkgcc -S' of the
#                unit, reading the emitted table's labels in ADDRESS order
#                against the listing's -- see docs/matching-levers.md under
#                "ARM ORDER IS DIRECTLY MEASURABLE". This tool still does not
#                chase them, but not for the reason it claimed.
# ONLY `%lo' OCCURRENCES COUNT, and the reason is arithmetic rather than
# convention.  A `%hi' is PAGE-GRANULAR: D_8006CCD8 and D_8006CCE8 are 0x10
# apart and both have %hi = 0x8007, so `lui $v0, %hi(D_8006CCD8)' and
# `lui $v0, %hi(D_8006CCE8)' assemble to the SAME word.  A `%hi' therefore
# carries no symbol identity within a 64K page, which is exactly the range in
# which a wrong-bank-table defect lives.
#
# Worse, the two sides SPELL an unpaired `%hi' differently.  When splat has no
# `%lo' to pair one with, it emits a bare immediate -- in batch 24,
# `lui $v0, (0x80070000 >> 16)' where the compiler writes
# `lui $2, %hi(D_8006CCE8)'.  Counting every occurrence made that a mismatch
# (build 4 `%hi' + 3 `%lo' against the listing's 3 + 3) on a body that is
# 656/656 words, 0 differing, in a byte-identical container.
#
# `%lo' is where a symbol's low 16 bits actually land, so it is the only half
# that distinguishes two symbols in one page -- and it still catches the defect
# this tool was built for: D_80175554 and D_8017555C differ in their `%lo'
# (0x5554 against 0x555C) even though their `%hi' is identical.
#
# AND THE BUILD SPELLS SOME ADDRESS LOADS `la', WHICH THIS PATTERN MISSED.
# gcc emits `la $8,D_800DEC28' -- one macro the assembler expands into the very
# `lui %hi' / `addiu %lo' pair this tool is looking for -- so a symbol reached
# that way carried NO `%lo' occurrence on the build side while the listing (which
# is always expanded) had one. That is a FALSE MISMATCH: the reference exists and
# is correct.
#
# Found 2026-11-26 on `func_b04_80172790', a body at 377/377 words, 0 differing,
# in five byte-identical containers -- which is itself the proof the relocation
# was right, since a wrong symbol cannot produce a byte-identical container. I
# came within one commit of treating a correct landing as defective, and the
# agent's own claim to have matched all twelve of its symbols by hand was
# accurate; this tool was not.
#
# Scope when fixed: 120 `la' references across 36 built units, every one a
# latent false positive. It had not fired before because it needs the listing to
# use `%lo' where the build uses `la'.
#
# A tool that reports a mismatch on correct input is worse than one that is
# silent, because the first thing it costs is trust in the landing.
# NOTE THE SHAPE: ONE capture group, with the alternation on the PREFIX. Both
# consumers below read `m.group(1)', so a two-branch alternation with a group in
# each branch would hand them None whenever the second branch matched -- and None
# would go straight into a Counter and a set as if it were a symbol name. That is
# the plausible-wrong-answer failure this project keeps paying for, so the prefix
# alternation is not a style choice.
SYM = re.compile(r"(?:%lo\(|\bla\s+\$[0-9a-z]+,\s*)"
                 r"(D_(?:[a-z0-9]+_)?8[0-9A-Fa-f]{7})")
# Call targets: `jal <name>' on either side. Bank-local (func_b38_8017...) and
# plain address forms (func_80069ef8) both match; so do real names.
CALL = re.compile(r"\bjal\s+([A-Za-z_][A-Za-z0-9_]*)")


def name_map():
    """{lowercase addr-form name: canonical name} from splat/common_syms.txt."""
    out = {}
    path = os.path.join(PROJ, "splat", "common_syms.txt")
    if not os.path.exists(path):
        return out
    for line in io.open(path, encoding="utf-8", errors="replace"):
        m = re.match(r"\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(0x[0-9a-fA-F]+)\s*;", line)
        if m:
            out["func_%s" % m.group(2)[2:].lower()] = m.group(1)
    return out


NAMES = None


def canon(sym):
    """Canonicalise a call target through common_syms.txt, case-insensitively."""
    global NAMES
    if NAMES is None:
        NAMES = name_map()
    low = sym.lower()
    if low in NAMES:
        return NAMES[low]
    # an address-form name is spelled with either case in the two streams
    if re.match(r"^func_8[0-9a-f]{7}$", low):
        return low
    return sym


def calls(text):
    return collections.Counter(canon(m.group(1)) for m in CALL.finditer(text))
# Resident code starts at 0x8003c9f8; nothing below that can be a bank table.
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


def interesting(name):
    try:
        return int(name[-8:], 16) >= LOW
    except ValueError:
        return True


def listing_syms(unit, func):
    p = os.path.join(PROJ, "asm", "nonmatchings", unit, func + ".s")
    try:
        t = open(p, errors="replace").read()
    except OSError:
        return None
    # A jump table BELONGS to the function, it is not a reference to one, and
    # the listing defines it in a `.section .rodata' block at the head.
    t = re.sub(r"\.section \.rodata.*?\.section \.text", "", t, flags=re.S)
    return (collections.Counter(m.group(1) for m in SYM.finditer(t)
                                if interesting(m.group(1))),
            calls(t))


def built_syms(unit):
    """{func: Counter(symbols)} from the built assembly, per function body."""
    p = os.path.join(PROJ, "build", "csrc", unit + ".s")
    try:
        lines = open(p, errors="replace").read().split("\n")
    except OSError:
        return None
    out, cur, buf = {}, None, []
    for ln in lines:
        m = re.match(r"^(func_\w+):", ln)
        if m:
            cur, buf = m.group(1), []
            continue
        if re.match(r"^\s*\.end\b", ln) and cur:
            body = "\n".join(buf)
            out[cur] = (collections.Counter(
                x.group(1) for x in SYM.finditer(body)
                if interesting(x.group(1))), calls(body))
            cur = None
            continue
        if cur is not None:
            buf.append(ln)
    return out


def check(unit):
    built = built_syms(unit)
    if built is None:
        return None
    rows = []
    for func, (got, gotc) in sorted(built.items()):
        w = listing_syms(unit, func)
        if w is None:
            continue
        want, wantc = w
        missing = want - got
        extra = got - want
        cmissing = wantc - gotc
        cextra = gotc - wantc
        if missing or extra or cmissing or cextra:
            rows.append((func, missing, extra, cmissing, cextra))
    return len(built), rows


def main():
    units = sys.argv[1:] or sorted(
        os.path.basename(p)[:-2]
        for p in glob.glob(os.path.join(PROJ, "build", "csrc", "ovl_dng_*.s")))
    nf = nb = 0
    nskip = 0
    for unit in units:
        r = check(unit)
        if r is None:
            # A FUNCTION NAME GETS ITS OWN MESSAGE. This tool takes UNIT
            # names, and handed `func_s42_80024AD0' it printed
            # "func_s42_80024AD0 NOT BUILT -- run make check first" -- after a
            # make check that had just succeeded. I did that on 2026-11-26,
            # one hour after fixing the identical confusion in declcheck for
            # an agent, and spent four commands looking for a build problem
            # that did not exist. The message was true (there is no unit by
            # that name, so nothing is built for it) and useless.
            if unit.startswith("func_"):
                parts = unit.split("_")
                guess = ("ovl_dng_" + parts[1]) if len(parts) > 2 else "<unit>"
                print("%-22s is a FUNCTION name; this tool takes UNIT names "
                      "-- try `%s'" % (unit, guess))
            else:
                print("%-22s NOT BUILT -- run make check first" % unit)
            nskip += 1
            continue
        n, rows = r
        nf += n
        for func, missing, extra, cmissing, cextra in rows:
            nb += 1
            print("%s: %s" % (unit, func))
            if missing:
                print("    DATA the LISTING references, the build does not: %s"
                      % ", ".join(sorted(missing.elements())))
            if extra:
                print("    DATA the BUILD references, the listing does not: %s"
                      % ", ".join(sorted(extra.elements())))
            if cmissing:
                print("    CALLS the LISTING makes, the build does not: %s"
                      % ", ".join(sorted(cmissing.elements())))
            if cextra:
                print("    CALLS the BUILD makes, the listing does not: %s"
                      % ", ".join(sorted(cextra.elements())))
    print("\n%d landed bodies checked, %d with a data-symbol or call mismatch"
          % (nf, nb))
    # A SKIPPED UNIT USED TO READ AS A PASS. Unbuilt units `continue', so a run
    # where NOTHING could be checked printed "0 landed bodies checked, 0 with a
    # mismatch" -- indistinguishable from a clean sweep. An agent reported it as
    # exactly the checks-that-pass-wrongly shape, and it is the sixth of that
    # family in this project. Say so loudly and exit non-zero.
    if nskip:
        print("%d unit(s) SKIPPED as NOT BUILT -- this run did not check them."
              % nskip)
        if nf == 0:
            raise SystemExit(
                "relocheck: NOTHING WAS CHECKED (every unit unbuilt). Run "
                "`make check' first; a 0/0 result is not a pass.")
        raise SystemExit(2)


if __name__ == "__main__":
    main()
