#!/usr/bin/env python3
"""nmcheck.py [FILE...] -- check docs/near-misses/ still compiles and still says the truth.

Those files are the project's resume points: the README tells you to copy one
to a zz_*.c scratch file and run verify.py on it. So two things can rot
silently, and both have:

  * the file stops compiling. A `*/` inside the header comment -- `s32*/u32*`
    was the real case -- terminates it early and the rest of the header is
    parsed as C. Nothing notices, because nothing compiles these files.
  * the header's claimed word/diff count drifts from what the source actually
    produces, after include/structs.h or a prototype changes under it.

It also warns -- separately, as a LANDING CHECKLIST and not an error -- where a
park's own signature disagrees with the prototype for that function in csrc/.
The prototype in scope is what every caller compiled against, so landing a
park whose signature differs rewrites those callers; func_8003AB44's landing
needed exactly that noticing, where a missing declaration had been passing a
`u8` argument untruncated. These are NOT evidence about parameter widths --
checked 2026-10-28 across all twelve current mismatches, and in every one the
caller's own argument is already the narrower type, so the declared width
changes no caller code either way.

This parses "func_NNNNNNNN (unit_X) -- A of B words, N DIFF" out of each
header's first lines, compiles the file, and compares. Exit status is non-zero
if any file fails to build or disagrees with its own header.

    python3 tools/decomp/nmcheck.py
    python3 tools/decomp/nmcheck.py docs/near-misses/func_80042900.c
"""
import glob
import io
import os
import re
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.environ.get("AZURE_ROOT", os.path.dirname(os.path.dirname(HERE)))

# "func_80042900 (unk_14934) -- 1 instruction off, 34 words against 33."
# "func_80045CC4 (unk_14934) -- 436 of 437 words, 47 DIFF."
HEAD = re.compile(r"(func_[0-9A-Fa-f]{8})\s*\((\w+)\)")


def unit_gas_flags():
    """unit -> the verify.py flag matching how the Makefile assembles it.

    The carved units are built with a nonzero gas -G, so verifying them at the
    project default would model the wrong assembler and report a false failure.
    Read the mapping out of the Makefile rather than hardcoding it, so adding a
    carve cannot silently break this check.
    """
    mk = open(os.path.join(PROJ, "Makefile")).read()
    out = {}
    for m in re.finditer(r"^((?:\s*build/csrc/\S+\.o\s*\\?\s*)+):\s*"
                         r"MASPSXFLAGS\s*:=\s*\$\(MASPSXFLAGS_G(\d+)\)",
                         mk, re.M):
        for u in re.findall(r"build/csrc/(\S+?)\.o", m.group(1)):
            out[u] = "--gas-G=" + m.group(2)
    return out
CLAIM = re.compile(r"(\d+)\s+of\s+(\d+)\s+words,\s*(\d+)\s+DIFF")
# some headers phrase it the other way round: "34 words against 33"
CLAIM_W = re.compile(r"(\d+)\s+words\s+against\s+(\d+)")
# a file holding several functions opts out of the per-function verify
MULTI = re.compile(r"nmcheck:\s*multi")
BUILT = re.compile(r"built (\d+) words, target (\d+) words")
RESULT = re.compile(r"RESULT: (?:(\d+) DIFF|MATCH)")


def compile_only(path, text):
    """For a file holding several functions: does it still compile at all?"""
    fd, tmp = tempfile.mkstemp(suffix=".c", prefix="zznm_", dir=PROJ)
    try:
        with os.fdopen(fd, "w") as fh:
            fh.write(text)
        r = subprocess.run(
            [os.path.join(PROJ, "tools/ccpsx"), "-O2", "-G8", "-Iinclude", "-Ipsyq/include", "-S",
             "-o", "/dev/null", os.path.basename(tmp)],
            cwd=PROJ, capture_output=True, text=True)
    finally:
        os.unlink(tmp)
    if r.returncode:
        first = (r.stdout + r.stderr).strip().split("\n")[0][:90]
        return "does not compile (%s)" % first
    return None


def check(path):
    text = open(path).read()
    head = text[:4000]
    if MULTI.search(head):
        problem = compile_only(path, text)
        return path, "(multi)", problem or "compiles; several functions, no single count to check"
    m = HEAD.search(head)
    if not m:
        return path, None, "no 'func_XXXXXXXX (unit)' in the header"
    func, unit = m.group(1), m.group(2)
    claim = CLAIM.search(head)
    claim_w = None if claim else CLAIM_W.search(head)

    fd, tmp = tempfile.mkstemp(suffix=".c", prefix="zznm_", dir=PROJ)
    try:
        with os.fdopen(fd, "w") as fh:
            fh.write(text)
        cmd = ["python3", os.path.join(HERE, "verify.py"),
               os.path.basename(tmp), unit, func]
        flag = unit_gas_flags().get(unit)
        if flag:
            cmd.append(flag)
        r = subprocess.run(cmd, cwd=PROJ, capture_output=True, text=True)
    finally:
        os.unlink(tmp)

    mb, mr = BUILT.search(r.stdout), RESULT.search(r.stdout)
    if not mb or not mr:
        tail = (r.stdout + r.stderr).strip().split("\n")[-1][:90]
        return path, func, "does not build (%s)" % tail
    built, target = int(mb.group(1)), int(mb.group(2))
    diff = int(mr.group(1) or 0)
    got = "%d of %d words, %d DIFF" % (built, target, diff)
    if claim:
        want = (int(claim.group(1)), int(claim.group(2)), int(claim.group(3)))
        if want != (built, target, diff):
            return path, func, "header says %d of %d words, %d DIFF; actual %s" % (
                want[0], want[1], want[2], got)
        return path, func, None
    if claim_w:
        want = (int(claim_w.group(1)), int(claim_w.group(2)))
        if want != (built, target):
            return path, func, "header says %d words against %d; actual %s" % (
                want[0], want[1], got)
        return path, func, None
    return path, func, "builds (%s); header states no count" % got


def unit_for_addr(addr):
    """Which c subsegment currently owns this vram address, per the splat spec.

    Carving a $gp cluster moves every later function into a renamed unit, so a
    near-miss header's UNIT goes stale every time -- four times so far. This
    derives it instead of anyone remembering to update it.
    """
    y = open(os.path.join(PROJ, "splat/slus_006.14.yaml")).read()
    subs = sorted((int(m.group(1), 16), m.group(2))
                  for m in re.finditer(r"^\s*-\s*\[(0x[0-9a-fA-F]+),\s*c,\s*(\w+)\]",
                                       y, re.M))
    off = addr - 0x8002d000 + 0x800
    owner = None
    for start, name in subs:
        if start <= off:
            owner = name
        else:
            break
    return owner


def fix_units():
    """Rewrite each near-miss header's UNIT to the one that owns its address."""
    changed = 0
    for path in sorted(glob.glob(os.path.join(PROJ, "docs/near-misses/*.c"))):
        text = open(path).read()
        m = HEAD.search(text[:4000])
        if not m:
            continue
        func, unit = m.group(1), m.group(2)
        want = unit_for_addr(int(func[5:], 16))
        if want and want != unit:
            open(path, "w").write(text.replace("(%s)" % unit, "(%s)" % want, 1))
            print("  %-34s %s -> %s" % (os.path.relpath(path, PROJ), unit, want))
            changed += 1
    print("%d header(s) repointed" % changed)
    return 0


def signature_warnings(files):
    """Park signatures that disagree with the prototype in csrc/."""
    csrc = "\n".join(io.open(q, encoding="utf-8", errors="replace").read()
                     for q in glob.glob(os.path.join(PROJ, "csrc/*.c")))
    out = []
    for path in files:
        func = os.path.basename(path)[:-2]
        text = io.open(path, encoding="utf-8", errors="replace").read()
        d = re.search(r"^[A-Za-z_][\w \*]*\b%s\s*\(([^)]*)\)\s*$" % re.escape(func),
                      text, re.M)
        if not d:
            continue
        mine = ([re.sub(r"\s+\w+$", "", x).strip().replace(" ", "")
                 for x in d.group(1).split(",")]
                if d.group(1).strip() not in ("", "void") else [])
        for proto in set(re.findall(
                r"^[A-Za-z_][\w \*]*\b%s\s*\(([^)]*)\)\s*;" % re.escape(func),
                csrc, re.M)):
            theirs = ([x.strip().replace(" ", "") for x in proto.split(",")]
                      if proto.strip() not in ("", "void") else [])
            if mine != theirs:
                out.append((func, d.group(1).strip(), proto.strip()))
    return sorted(set(out))


def main():
    if "--fix-units" in sys.argv:
        return fix_units()
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    files = args or sorted(glob.glob(os.path.join(PROJ, "docs/near-misses/*.c")))
    bad = 0
    for path in files:
        rel = os.path.relpath(path, PROJ)
        _, func, problem = check(path)
        if problem is None:
            print("ok    %-34s %s" % (rel, func))
        elif problem.startswith("builds") or problem.startswith("compiles") \
                or "states no count" in problem:
            print("note  %-34s %s" % (rel, problem))
        else:
            bad += 1
            print("FAIL  %-34s %s" % (rel, problem))
    print()
    print("%d file(s), %d problem(s)" % (len(files), bad))
    warn = signature_warnings(files)
    if warn:
        print("\n%d signature(s) to correct when the park lands (not an error):"
              % len(warn))
        for func, mine, theirs in warn:
            print("  %-16s park(%s)" % (func, mine))
            print("  %-16s csrc(%s)" % ("", theirs))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
