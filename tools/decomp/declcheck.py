#!/usr/bin/env python3
"""declcheck.py -- does any landed body call a function with no declaration in
scope?  Compiles each unit standalone and reports what the compiler warns about.

## THE GAP THIS CLOSES

An IMPLICITLY DECLARED function is invisible to every other check in this
tree.  K&R C gives it `int (...)', promotes its arguments and takes its result
as `int' -- and the body still compiles, still links, and in many cases still
produces the right words, so `ovlverify' reads 0 differing and `relcand' reads
ok.  Found by agent-h106 in batch 26: `func_s69_80024020' calls
`func_s69_800244C4', which is DEFINED LOWER IN THE SAME FILE and was never
declared.  That body measured 194/194/0 undeclared.

It was right only by luck -- both arguments were already pointers and the
result went straight to an `sw'.  Change any of those and the promotion
changes the code while every tool stays quiet.  This is the fourth way a
measurement can read falsely in this project, and the only one no existing
check can see.

**The gap is STRUCTURAL, not a slip.**  A unit's declaration block is written
by whoever lands the first body that needs a symbol, so a body calling a
sibling defined LATER in its own file has nothing in scope -- and nothing
complains, because the definition satisfies the linker.

## WHY A STANDALONE COMPILE

`make' builds with the project's flags and does not surface these.  A direct
`tools/cdkgcc -S' over the unit is the only thing in the toolchain that prints
`warning: implicit declaration of function' and
`warning: type mismatch with previous implicit declaration', and it costs about
two seconds per unit.  It also catches argument-type mismatches against a real
prototype -- h106's `RotTrans' third argument wanted `long *' where the body
passed `s32 *'.

Usage:
    python3 tools/decomp/declcheck.py                 # every unit
    python3 tools/decomp/declcheck.py ovl_dng_s69     # one unit
"""
import glob
import os
import re
import subprocess
import sys
import tempfile

PROJ = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
CC = os.path.join(PROJ, "tools", "cdkgcc")
# the warnings that mean a callee's type is not known at the call site
WANT = re.compile(r"(implicit declaration|type mismatch with previous implicit|"
                  r"conflicting types|passing arg \d+ of)", re.I)


def check(unit, tmp):
    src = os.path.join(PROJ, "csrc", unit + ".c")
    if not os.path.exists(src):
        return None
    # The Makefile's own flags, RELATIVE, run from the project root -- psyq's
    # headers do not parse with absolute -I paths from another directory.
    cmd = [CC, "-S", "-O2", "-Iinclude", "-Ipsyq/include", "-G0",
           "-o", os.path.join(tmp, unit + ".s"),
           os.path.join("csrc", unit + ".c")]
    r = subprocess.run(cmd, capture_output=True, text=True, cwd=PROJ)
    out = (r.stderr or "") + (r.stdout or "")
    hits = [l.strip() for l in out.splitlines() if WANT.search(l)]
    return hits, r.returncode, out


def main():
    units = sys.argv[1:] or sorted(
        os.path.basename(p)[:-2]
        for p in glob.glob(os.path.join(PROJ, "csrc", "ovl_dng_*.c")))
    nu = nbad = nhit = nfail = 0
    with tempfile.TemporaryDirectory() as tmp:
        for unit in units:
            r = check(unit, tmp)
            if r is None:
                print("%-24s NO SUCH UNIT" % unit)
                continue
            hits, rc, out = r
            nu += 1
            if rc != 0 and not hits:
                nfail += 1
                print("%-24s did not compile standalone (rc=%d)" % (unit, rc))
                for l in out.splitlines()[:3]:
                    print("    %s" % l.strip())
                continue
            if hits:
                nbad += 1
                nhit += len(hits)
                print("%s:" % unit)
                for l in sorted(set(hits)):
                    print("    %s" % l)
    print("\n%d unit(s) checked, %d with an undeclared or mistyped callee "
          "(%d warning(s)), %d that did not compile standalone"
          % (nu, nbad, nhit, nfail))
    return 1 if nbad else 0


if __name__ == "__main__":
    sys.exit(main())
