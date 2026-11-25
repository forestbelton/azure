#!/usr/bin/env python3
"""ovlverify for a function the carve split across SEVERAL symbols.

ovl_dng_lo20's symbol boundaries are wrong: a single function can carry two or
four glabels, so `ovlverify' measures the head's listing (4 words, say) against
the whole 17-word body and reports a mismatch however right the body is. This
measures the candidate's HEAD symbol against the CONCATENATION of the listings,
in listing order, with ovlverify's own masking and reporting.

    python3 pairverify.py <candidate.c> <head> <next> [<next> ...]

The head must be DEFINED in the candidate and every other name must have had
its INCLUDE_ASM removed, so that the one definition covers all of their words.

WHY THIS CANNOT BE FOLDED INTO ovlverify: ovlverify takes its target extent
from the per-function listing and stops at the next symbol, which is exactly
the wrong behaviour when the next symbol is spurious.  Eleven of twenty-two
names handed to one agent were fragments, and ten of them measured 0 differing
here after joining while ovlverify could not measure them at all.

HOW TO KNOW YOU NEED IT -- count `jr $ra' in each listing:
  none at all          -> fragment HEAD (rule out a noreturn tail call first)
  the symbol after one -> fragment TAIL
  two OR MORE in a
  small listing        -> that many functions under one symbol
A chain can be longer than two: four consecutive lo20 symbols are one 350-word
function, forced by branch targets landing inside each other.

Run project-wide this defect is confined to ovl_dng_lo20; the other 115 carved
objects are clean.

PAIRVERIFY_FLAGS=--raw for raw mode; PAIRVERIFY_SPLIT=1 when the candidate
defines each part as its own C function.
"""
import os, re, subprocess, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import asmlib, ovlverify

def main():
    src, names = sys.argv[1], sys.argv[2:]
    ovlverify.SRC_UNIT = os.path.splitext(os.path.basename(src))[0]
    unit = ovlverify.overlay_for(names[0])
    words, relocs = asmlib.build(src, gasG=0, cflags=("-G0",), unit=unit)
    o = os.path.join(asmlib.procdir(),
                     os.path.splitext(os.path.basename(src))[0] + ".o")
    syms = {}
    for ln in subprocess.run(["mips64-linux-gnu-nm", "-n", o],
                             capture_output=True, text=True).stdout.split("\n"):
        m = re.match(r"([0-9a-f]+) [Tt] (\S+)", ln)
        if m:
            syms[m.group(2)] = int(m.group(1), 16)
    txt = open(src).read()
    for n in names[1:]:
        if re.search(r'INCLUDE_ASM\("[^"]*",\s*%s\s*\)' % re.escape(n), txt):
            sys.exit("%s is still an INCLUDE_ASM -- remove it first" % n)
        if n in syms and not os.environ.get("PAIRVERIFY_SPLIT"):
            sys.exit("%s is still a symbol in the object -- the head's extent "
                     "stops there (set PAIRVERIFY_SPLIT=1 if the candidate "
                     "deliberately defines each part as its own C function)" % n)
    head = names[0]
    if head not in syms:
        sys.exit("%s not defined in %s" % (head, src))
    lo = syms[head]
    last = lo
    if os.environ.get("PAIRVERIFY_SPLIT"):
        # The candidate defines each part as its own C function, in order, so
        # the head's extent has to run past them all. Require the order.
        for n in names[1:]:
            if n not in syms:
                sys.exit("%s not defined in %s" % (n, src))
            if syms[n] <= last:
                sys.exit("%s is not emitted after the previous part" % n)
            last = syms[n]
    later = sorted(v for v in syms.values() if v > last)
    hi = later[0] if later else max(a for a, _, _ in words) + 4
    body = [(a, w, t) for a, w, t in words if lo <= a < hi]
    tgt = []
    for n in names:
        # A part that splat named as DATA (an `alabel' inside the head's own
        # listing) has no listing of its own, and the head's listing already
        # carries its words -- so skip it rather than dying.
        try:
            tgt += ovlverify.target(n)
        except (ValueError, SystemExit):
            print("  (%s has no listing of its own; the head's listing covers "
                  "it)" % n)
    raw = "--raw" in os.environ.get("PAIRVERIFY_FLAGS", "")
    d = ovlverify.report("+".join(names), body, relocs, tgt, not raw)
    sys.exit(1 if d or len(body) != len(tgt) else 0)

main()
