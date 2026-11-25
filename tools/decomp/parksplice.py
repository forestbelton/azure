#!/usr/bin/env python3
"""parksplice.py FUNC OUT.c -- write the CURRENT unit with FUNC's park spliced in.

parkresidue.py does this into a temp directory and throws it away, which is
right for a ranking sweep and wrong for everything else.  Two jobs need the
spliced unit to PERSIST:

  * `permimport.py --source` wants a file that defines the function and
    compiles, and for a parked function that is exactly this;
  * an agent picking up a park wants to start from the measured body rather
    than re-deriving the splice and the declaration block by hand.

Both were being done by copy-paste, and a splice done by hand is where the
declarations get dropped -- that is how 19 of 34 parks stopped compiling.

    python3 tools/decomp/parksplice.py func_800C5FA8 /tmp/x/ovl_dungeon.c
    python3 tools/decomp/aligndiff.py /tmp/x/ovl_dungeon.c func_800C5FA8

KEEP THE BASENAME `ovl_dungeon.c`.  ovlverify.py picks which container ELF to
resolve against from the source file's BASENAME; anything else silently falls
back to build/town.bin.elf.  This script refuses another name rather than let
that happen quietly.
"""
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import parkresidue as pr


def main():
    if len([a for a in sys.argv[1:] if not a.startswith("--")]) != 2:
        print(__doc__)
        return 2
    func, out = sys.argv[1], sys.argv[2]
    alt = "--alt" in sys.argv
    want = os.path.basename(pr.unit_of(func)[0] or "ovl_dungeon.c")
    if os.path.basename(out) != want:
        print("%s: the output basename must be %s (ovlverify picks "
              "the container ELF from it); got %r"
              % (out, want, os.path.basename(out)))
        return 2
    # SEARCH BOTH PARK DIRECTORIES. This hardcoded `docs/dungeon-parks/' --
    # the OLD block's directory -- so it could not find any of the twelve
    # carved parks in `docs/dng-parks/' and reported "no park at ..." for every
    # one of them. parkresidue.py already resolves both through PARK_DIRS;
    # reuse it rather than keep a second copy of the path. Third tool today
    # still wired to the pre-carve layout, after permimport's name pattern and
    # dngcallee's unit glob.
    park = None
    for d in pr.PARK_DIRS:
        cand = os.path.join(pr.PROJ, d, "%s.c" % func)
        if os.path.exists(cand):
            park = cand
            break
    if park is None:
        print("%s: no park in %s"
              % (func, " or ".join(pr.PARK_DIRS)))
        return 1
    body = pr.body_of(park, func)
    if body is None:
        print("%s: no definition line in the park" % func)
        return 1
    unit = open(pr.unit_of(func)[0], encoding="utf-8").read()
    stub = 'INCLUDE_ASM("asm/nonmatchings/%s", %s);' % (pr.unit_of(func)[1], func)
    if unit.count(stub) != 1:
        print("%s: not a stub in the unit (landed already?)" % func)
        return 1
    decls = pr.decls_of(park) + pr.preamble_of(park, func)

    # THE SYNTHESISED SELF-PROTOTYPE IS A MEASUREMENT AID THAT CAN BREAK A
    # LANDED FUNCTION, SO SAY SO BEFORE ANYONE MERGES THIS FILE.
    #
    # pr.splices() appends the park's own signature as a forward declaration
    # (`decls + sig + ";"'), which is right for MEASURING: without it, a call
    # hundreds of lines above the body draws "type mismatch with previous
    # implicit declaration" and the park will not compile at all.
    #
    # But a caller that currently reaches this function through an implicit
    # `int f()' is relying on DEFAULT ARGUMENT PROMOTIONS, and a real
    # prototype changes them. On 2026-11-22, splicing func_800ADE74 -- whose
    # signature takes `u16 d, u16 e' -- made its only in-unit caller,
    # the LANDED func_800ADDA0, go from 53/53 and 0 differing to 52 candidate
    # words and 50 DIFFERING: its s16 arguments are SIGN-extended through the
    # implicit declaration, which is what the target has, and ZERO-extended
    # through the u16 prototype. func_800ADE74 itself measures identically
    # either way.
    #
    # So the file this tool writes is correct to MEASURE and wrong to MERGE,
    # which is the most dangerous shape a tool output can have. Warn whenever
    # the unit calls the function above the splice point and does not already
    # declare it -- exactly the case where adding the prototype changes a
    # caller.
    _stub_at = unit.find(stub)
    _above = unit[:_stub_at] if _stub_at > 0 else ""
    _calls_above = len(re.findall(r"\b%s\s*\(" % re.escape(func), _above))
    _declared = re.search(r"^[A-Za-z_][^\n;]*\b%s\s*\([^;]*;" % re.escape(func),
                          _above, re.M) is not None
    if _calls_above and not _declared:
        print("%s: *** %d call site(s) above the body reach this function "
              "through an IMPLICIT declaration, and this splice adds a real "
              "prototype." % (func, _calls_above))
        print("%s: *** That changes their DEFAULT ARGUMENT PROMOTIONS. Measure "
              "those callers with ovlverify before you merge, and omit the "
              "synthesised `%s(...);' line if any of them moves." % (func, func))
        print("%s: *** It cost func_800ADDA0 50 differing rows on 2026-11-22; "
              "the park is correct to MEASURE and wrong to MERGE." % func)
    # SAME placement and prototype-adoption logic as parkresidue.measure, and
    # it has to stay the same: this file is what an agent picks up and what
    # permimport --source compiles, so a splice that differs from the one the
    # ranking measured hands them a different function to work on.
    cands = list(pr.splices(unit, stub, body, decls))
    if alt:
        cands = cands[-1:]
    d = os.path.dirname(os.path.abspath(out))
    if d:
        os.makedirs(d, exist_ok=True)

    # TRY EACH PLACEMENT AND KEEP THE ONE THAT COMPILES, rather than writing
    # the first and leaving the caller to discover it does not. On 2026-11-16
    # an agent was handed the default placement for a park whose typedefs
    # build on UnkDungeonSPXY -- declared ~900 lines below the top block -- and
    # had to work out for itself that `--alt` was the one that worked.
    # The measurement is worth having anyway: it hands the caller a verified
    # starting residue instead of a file and a promise.
    env = dict(os.environ)
    env.setdefault("AZURE_SCRATCH", os.path.join(d or ".", "sp"))
    os.makedirs(env["AZURE_SCRATCH"], exist_ok=True)
    last = None
    for i, text in enumerate(cands):
        open(out, "w").write(text)
        r = subprocess.run(
            [sys.executable, "tools/decomp/aligndiff.py", out, func],
            capture_output=True, text=True, env=env, cwd=pr.PROJ)
        m = re.search(r"built (\d+) words, target (\d+) words, (\d+) aligned",
                      r.stdout)
        if m:
            print("%s: spliced into %s (%d declaration line(s), placement %d "
                  "of %d) -- %s words, %s aligned differences"
                  % (func, out, len(decls.strip().split("\n")) if decls else 0,
                     i + 1, len(cands), m.group(1), m.group(3)))
            if m.group(1) != m.group(2):
                print("  NOTE: target is %s words; this body is %s."
                      % (m.group(2), m.group(1)))
            return 0
        # KEEP THE WHOLE TAIL, NOT THE LINES CONTAINING "error".
        # cc1psx's own diagnostics frequently contain neither "error" nor
        # "undeclared" -- "unterminated character constant", "parse error
        # before ...", "conflicting types for ..." -- so this filter threw
        # the message away and printed "Last error:" with nothing after it.
        # Two parks were unmeasurable for a day with no diagnosis available,
        # and the filter was why.
        raw = (r.stderr or "") + (r.stdout or "")
        tail = [l for l in raw.strip().split("\n") if l.strip()]
        last = "\n  ".join(tail[-6:])[:900] if tail else "did not compile"
    print("%s: NO placement compiles. Last error:\n  %s" % (func, last))
    print("The park needs its DECLARATIONS block repaired before it can be "
          "measured; see docs/dungeon-parks/README.md.")
    return 1



if __name__ == "__main__":
    sys.exit(main())
