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


def _norm(t):
    return re.sub(r"\s+", "", t)


def decl_units(text):
    """Whole declarations: a typedef spans lines and must not be split.

    The first version of this filter worked per LINE, and a typedef's field
    lines (`u16 unk_64;') occur verbatim in other structs -- so it deleted
    them out of the park's own typedef and left `typedef struct X' followed
    by `}', a syntax error at the splice point. Brace depth is what separates
    one declaration from the next.
    """
    out, cur, depth = [], [], 0
    for line in (text or "").split("\n"):
        cur.append(line)
        depth += line.count("{") - line.count("}")
        if depth <= 0 and line.strip().endswith(";"):
            out.append("\n".join(cur))
            cur, depth = [], 0
    if cur:
        out.append("\n".join(cur))
    return out


def drop_known(decls, unit, func="", quiet=False):
    """Drop any park declaration the UNIT already carries, and say which.

    A declaration the unit already has is by definition not needed, so
    dropping it is safe in every case and silent when there is nothing to
    drop. Without this, a sibling landing that adds a typedef to the unit
    makes the park stop compiling -- `redefinition of struct Unk...' -- and
    the park looks stale when it is fine.

    SHARED, because it was a closure here and `parkresidue.py' did not have
    it: a measured ranking of 40 parks skipped FOUR of them for exactly that
    redefinition, and `parksplice' spliced the same parks cleanly.
    """
    unit_norm = _norm(unit)
    # MATCHING BY EXACT TEXT WAS NOT ENOUGH. A unit that has since REFINED a
    # typedef the park also declares -- renamed a field, resized a pad -- no
    # longer contains the park's spelling, so the park's copy survived and the
    # splice died on `redefinition of struct Unk...'. Measured 2026-11-25: SIX
    # of ovl_dng_lo16's nineteen parks could not be measured at all for this
    # reason, which is invisible in any park inventory because the figures were
    # recorded when the park still compiled.
    #
    # So a struct/union/enum declaration is now matched BY TAG. If the unit
    # defines that tag, the unit's definition is the current truth and the
    # park's copy goes, whether or not the two agree. They cannot both be
    # present, and keeping the park's is not an option that compiles.
    tags = set(re.findall(r"\b(?:struct|union|enum)\s+([A-Za-z_]\w*)\s*\{",
                          unit))
    kept, dropped, refined, protos, tagdiff = [], [], [], [], []
    for decl in decl_units(decls):
        t = decl.strip()
        if t and len(_norm(t)) > 8 and _norm(t) in unit_norm:
            dropped.append(t.split("\n")[0])
            continue
        # SAME RULE FOR A DATA SYMBOL. A park's `extern s32 D_800E3D6C;'
        # against the unit's `extern u32 D_800E3D6C;' is `conflicting types',
        # which names the symbol but not which side to believe. The unit is
        # the current truth, so the park's declaration goes.
        m = re.match(r"extern\s+[^;{]*?\b([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*;",
                     t)
        if m and re.search(r"(?m)^\s*extern\b[^;{\n]*\b%s\b\s*(?:\[[^\]]*\])?\s*;"
                           % re.escape(m.group(1)), unit):
            refined.append("%s (the unit declares it with a different type; "
                           "using the unit's)" % m.group(1))
            dropped.append(t.split("\n")[0])
            continue
        # AND THE THIRD KIND: A FUNCTION PROTOTYPE. The unit's own block
        # declares the unit's stubs and callees, and a park that declares one
        # of them differently is `conflicting types' -- which blocked
        # func_lo16_800165B8 entirely. Dropping the park's copy is again the
        # only option that compiles, BUT unlike a typedef this can change
        # CODEGEN at the call: a parameter's width decides promotion. So this
        # one is reported LOUDLY and separately, because the park's recorded
        # figures were measured against ITS prototype and may not reproduce.
        m = re.match(r"(?:[A-Za-z_][\w \*]*?)\b([A-Za-z_]\w*)\s*\([^;{]*\)\s*;",
                     t)
        if m:
            fn = m.group(1)
            um = re.search(r"(?m)^\s*[A-Za-z_][\w \*]*?\b%s\s*\(([^;{]*)\)\s*;"
                           % re.escape(fn), unit)
            if um:
                protos.append((fn, " ".join(t.split()),
                               " ".join(um.group(0).split())))
                dropped.append(t.split("\n")[0])
                continue
        m = re.search(r"\b(?:struct|union|enum)\s+([A-Za-z_]\w*)\s*\{", t)
        if m and m.group(1) in tags:
            # DROPPING IS STILL THE ONLY OPTION THAT COMPILES -- the two
            # definitions cannot both be present. But whether that is HARMLESS
            # or changes the park's figures depends on whether they AGREE, and
            # reporting both cases the same way hid a real fault: a park whose
            # own fact was about a field it had refined to `s8' got spliced
            # against the unit's `u8', and the extra aligned row that produced
            # was recorded as the park's own residue. Its real baseline was
            # 264/3, not 264/4 -- one of its four rows was never that body's
            # fault, and the park had been worked against the wrong number.
            #
            # A field width decides promotion and sign-extension, so a
            # DIFFERING definition deserves the same loud treatment a
            # prototype gets. An identical one is genuinely nothing.
            tag = m.group(1)
            um = re.search(r"\b(?:struct|union|enum)\s+%s\s*\{[^}]*\}"
                           % re.escape(tag), unit)
            if um and _norm(um.group(0)) == _norm(m.group(0) + t[m.end():].split("}")[0] + "}"):
                dropped.append(t.split("\n")[0])
                continue
            tagdiff.append((tag,
                            " ".join(t.split())[:160],
                            " ".join(um.group(0).split())[:160] if um else "?"))
            dropped.append(t.split("\n")[0])
            continue
        kept.append(decl)
    if protos and not quiet:
        print("%s: %d PROTOTYPE(S) THE UNIT DECLARES DIFFERENTLY -- dropped the "
              "park's, kept the unit's. A parameter's width changes promotion "
              "at the call, so THIS PARK'S RECORDED FIGURES MAY NOT REPRODUCE:"
              % (func, len(protos)))
        for fn, parkv, unitv in protos:
            print("    %s\n      park: %s\n      unit: %s" % (fn, parkv, unitv))
    if tagdiff and not quiet:
        print("%s: %d TYPEDEF(S) THE UNIT DEFINES DIFFERENTLY -- dropped the "
              "park's, kept the unit's. A FIELD'S WIDTH CHANGES PROMOTION AND "
              "SIGN-EXTENSION, so THIS PARK'S RECORDED FIGURES MAY NOT "
              "REPRODUCE -- re-measure the baseline before trusting its table:"
              % (func, len(tagdiff)))
        for tag, parkv, unitv in tagdiff:
            print("    %s\n      park: %s\n      unit: %s" % (tag, parkv, unitv))
    if refined and not quiet:
        print("%s: %d park typedef(s) the unit has REDEFINED since: %s"
              % (func, len(refined), "; ".join(refined[:4])))
    if dropped and not quiet:
        print("%s: dropped %d declaration(s) the unit already has "
              "(park rot from a sibling landing): %s"
              % (func, len(dropped),
                 "; ".join(d[:48] for d in dropped[:4])
                 + (" ..." if len(dropped) > 4 else "")))
    return ("\n".join(kept) if dropped else decls), dropped


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

    # DROP ANY DECLARATION THE UNIT ALREADY HAS.
    #
    # A park's DECLARATIONS block is written against the AGENT's copy of the
    # unit. Every body that agent landed alongside the park then goes into
    # `csrc/', bringing its own typedefs and prototypes with it -- so the
    # park's copy becomes a REDEFINITION and the park stops splicing. This has
    # now happened three times, and the third was entirely predictable: the
    # park and the landed bodies came from the SAME agent in the SAME batch, so
    # the duplicate was created by integrating that agent's work.
    #
    # `dngland' already filters this way for the landing direction
    # (dngclone.reconcile_decls against the unit text); parksplice did not
    # filter at all. A declaration the unit already carries is by definition
    # not needed, so dropping it is safe in every case and silent when there is
    # nothing to drop.
    decls, _dropped = drop_known(decls, unit, func)

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
