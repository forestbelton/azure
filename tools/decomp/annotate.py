#!/usr/bin/env python3
"""annotate.py UNIT FUNC -- which matching levers this function can possibly need.

Reads the target listing and reports, per lever whose signature is present, the
sites, the doc section, the measured word cost of getting it wrong, the source
shapes to try, and the shapes already measured NOT to work.

It does NOT decide anything, and that is deliberate: the two worst detours in
this project's history came from inheriting a confident claim. Every line it
prints cites a measurement you can check, and the shapes it lists are the work
list for the next attempt -- not an answer.

Usage:
    python3 tools/decomp/annotate.py unk_14934 func_80042A80
    python3 tools/decomp/annotate.py unk_14934 func_80042A80 --sites
    python3 tools/decomp/annotate.py --self-test
"""
import os
import re
import sys

try:
    import yaml
except ImportError:
    sys.exit("annotate.py needs pyyaml (splat already requires it)")

PROJ = os.environ.get(
    "AZURE_ROOT",
    os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))),
)
LEVERS = os.path.join(os.path.dirname(os.path.abspath(__file__)), "levers.yaml")

# /* 16284 80042A84 6A2E4390 */  lbu        $v1, %lo(D_80082E6A)($v0)
INSN = re.compile(
    r"^\s*/\*\s*([0-9A-Fa-f]+)\s+([0-9A-Fa-f]+)\s+[0-9A-Fa-f]+\s*\*/\s*(\S+)\s*(.*?)\s*$"
)


class Insn:
    __slots__ = ("vram", "mnem", "ops", "text")

    def __init__(self, vram, mnem, ops):
        self.vram = vram
        self.mnem = mnem
        self.ops = ops
        self.text = (mnem + " " + ops).strip() if ops else mnem

    def __repr__(self):
        return "%s %s" % (self.vram, self.text)


def parse_listing(path):
    insns = []
    with open(path) as fh:
        for line in fh:
            m = INSN.match(line)
            if m:
                insns.append(Insn(m.group(2).upper(), m.group(3), m.group(4)))
    return insns


def step_matches(insn, step):
    """A window step is "mnem" or "mnem:imm"; mnem may be an a|b|c alternation."""
    if ":" in step:
        mnem, imm = step.split(":", 1)
        if insn.mnem not in mnem.split("|"):
            return False
        # accept 24, 0x18 and 0x24 spellings for the same shift count
        cands = {imm}
        try:
            n = int(imm, 0)
            cands |= {str(n), hex(n), "0x%X" % n}
        except ValueError:
            pass
        return any(c in insn.ops for c in cands)
    return insn.mnem in step.split("|")


def find_window(insns, steps, gap):
    """Every start index whose following insns hit `steps` in order."""
    hits = []
    for i in range(len(insns)):
        if not step_matches(insns[i], steps[0]):
            continue
        at = i
        ok = True
        for step in steps[1:]:
            found = None
            for j in range(at + 1, min(at + 2 + gap, len(insns))):
                if step_matches(insns[j], step):
                    found = j
                    break
            if found is None:
                ok = False
                break
            at = found
        if ok:
            hits.append((i, at))
    return hits


def find_match(insns, pattern):
    rx = re.compile(pattern)
    return [(i, i) for i, insn in enumerate(insns) if rx.search(insn.text)]


def keep_repeated(insns, hits, n):
    """Keep only hits whose exact instruction text occurs at least n times.

    That is the signature of reorg having duplicated one source insn, as
    opposed to an ordinary register copy, which is in nearly every function.
    """
    counts = {}
    for insn in insns:
        counts[insn.text] = counts.get(insn.text, 0) + 1
    return [h for h in hits if counts.get(insns[h[0]].text, 0) >= n]


def applicable(levers, insns):
    out = []
    for lv in levers:
        if "absent" in lv and find_match(insns, lv["absent"]):
            continue
        if "window" in lv:
            hits = find_window(insns, lv["window"], lv.get("gap", 2))
        elif "match" in lv:
            hits = find_match(insns, lv["match"])
        else:
            continue
        if "repeat" in lv:
            hits = keep_repeated(insns, hits, lv["repeat"])
        if hits:
            out.append((lv, hits))
    return out


def wrap(text, indent, width=78, hang=2):
    """Wrap, indenting continuation lines by `hang` extra spaces."""
    words = " ".join(str(text).split()).split(" ")
    cont = indent + " " * hang
    lines, cur = [], indent
    for w in words:
        if len(cur) + len(w) + 1 > width and cur.strip():
            lines.append(cur.rstrip())
            cur = cont
        cur += w + " "
    if cur.strip():
        lines.append(cur.rstrip())
    return "\n".join(lines)


def report(unit, func, levers, insns, show_sites):
    hits = applicable(levers, insns)
    print("%s / %s -- %d instructions" % (unit, func, len(insns)))
    if not hits:
        print()
        print("  No lever signature found. That means the listing shows none of the")
        print("  shapes in levers.yaml, not that the function is easy: statement")
        print("  structure, loop shape and register allocation leave no signature.")
        print("  Read docs/matching-levers.md sections 2, 3 and 8.")
        return 0
    print("%d lever(s) whose signature is present. These are candidates to CHECK,"
          % len(hits))
    print("not conclusions -- each one cites the measurement behind it.")
    for lv, sites in hits:
        print()
        print("=" * 78)
        print("%s  [%s]" % (lv["name"], lv["id"]))
        print("  %-10s %s" % ("doc:", lv["doc"]))
        print("  %-10s %s" % ("sites:", len(sites)))
        if show_sites:
            for a, b in sites[:12]:
                span = insns[a : b + 1]
                print("      %s  %s" % (span[0].vram,
                                        " ; ".join(i.text for i in span)))
            if len(sites) > 12:
                print("      ... %d more" % (len(sites) - 12))
        print(wrap("cost: " + lv["cost"], "  ", hang=8))
        print("  try, cheapest first:")
        for o in lv.get("options", []):
            print(wrap("- " + o, "    "))
        if lv.get("negative"):
            print("  measured NOT to work -- do not retry:")
            for n in lv["negative"]:
                print(wrap("- " + n, "    "))
        if lv.get("note"):
            print(wrap("note: " + lv["note"], "  "))
    return 0


def self_test(levers):
    """Check every lever's signature still fires where it should.

    Real functions are used where a stub still has a listing. Two signatures
    only occur in functions that have since been decompiled -- `make splat`
    deletes their listings -- so those use inline fixtures instead, which keeps
    the self-test independent of splat state.
    """
    FIXTURES = {
        "block-move": """
    /* 1 80042500 0 */  lb         $v0, 0x2C($v1)
    /* 1 80042504 0 */  lb         $v1, 0x2D($v1)
    /* 1 80042508 0 */  sb         $v0, 0x0($s0)
    /* 1 8004250C 0 */  sb         $v1, 0x1($s0)
""",
        "abs-single-insn": """
    /* 1 80046C00 0 */  bgez       $v0, .L80046C08
    /* 1 80046C04 0 */   nop
    /* 1 80046C08 0 */  subu       $v0, $zero, $v0
""",
        "join-materialisation": """
    /* 1 80043000 0 */  addu       $v0, $s2, $zero
    /* 1 80043004 0 */  lw         $v1, 0x1C($s1)
    /* 1 80043008 0 */  addu       $v0, $s2, $zero
""",
        "delay-slot-nop": """
    /* 1 80043100 0 */  lhu        $v0, 0x10($s0)
    /* 1 80043104 0 */  nop
    /* 1 80043108 0 */  sh         $v0, 0x80($s0)
""",
    }
    # The two noise-prone signatures need negative cases more than positive
    # ones: a lone register copy and a nop not in a load-delay slot are in
    # nearly every function, and matching them there would make the tool's
    # output worthless.
    NEGATIVE = {
        "join-materialisation": """
    /* 1 80043200 0 */  addu       $v0, $s2, $zero
    /* 1 80043204 0 */  lw         $v1, 0x1C($s1)
    /* 1 80043208 0 */  jr         $ra
""",
        "delay-slot-nop": """
    /* 1 80043300 0 */  addiu      $v0, $zero, 0x2
    /* 1 80043304 0 */  nop
    /* 1 80043308 0 */  jr         $ra
""",
    }
    cases = [
        ("unk_176B8", "func_80045CC4", "byte-fold"),
        ("unk_154BC", "func_80042A80", "srl-not-sra"),
        ("unk_176B8", "func_80047338", "gp-relative"),
        ("unk_154BC", "func_80042BDC", "constant-vs-symbol"),
        ("unk_154BC", "func_80042BDC", "switch-index-width"),
        ("unk_154BC", "func_80042BDC", "split-address-form"),
        ("unk_14934", "func_80041344", "loop-guard-blez"),
        ("unk_176B8", "func_80045CC4", "byte-decrement"),
        ("unk_176B8", "func_80045CC4", "halfword-signed-read"),
    ]
    bad = skipped = 0
    for unit, func, want in cases:
        path = "%s/asm/nonmatchings/%s/%s.s" % (PROJ, unit, func)
        if not os.path.exists(path):
            print("SKIP %-14s %-22s (no listing; run make splat)" % (func, want))
            skipped += 1
            continue
        ids = [lv["id"] for lv, _ in applicable(levers, parse_listing(path))]
        ok = want in ids
        bad += 0 if ok else 1
        print("%-4s %-14s expects %-22s %s"
              % ("ok" if ok else "FAIL", func, want,
                 "" if ok else "got: " + (", ".join(ids) or "-")))
    for want, text in FIXTURES.items():
        insns = [Insn(m.group(2).upper(), m.group(3), m.group(4))
                 for m in (INSN.match(l) for l in text.splitlines()) if m]
        ids = [lv["id"] for lv, _ in applicable(levers, insns)]
        ok = want in ids
        bad += 0 if ok else 1
        print("%-4s %-14s expects %-22s %s"
              % ("ok" if ok else "FAIL", "(fixture)", want,
                 "" if ok else "got: " + (", ".join(ids) or "-")))
    for want, text in NEGATIVE.items():
        insns = [Insn(m.group(2).upper(), m.group(3), m.group(4))
                 for m in (INSN.match(l) for l in text.splitlines()) if m]
        ids = [lv["id"] for lv, _ in applicable(levers, insns)]
        ok = want not in ids
        bad += 0 if ok else 1
        print("%-4s %-14s must NOT fire %-15s %s"
              % ("ok" if ok else "FAIL", "(negative)", want,
                 "" if ok else "but it did"))
    covered = {c[2] for c in cases} | set(FIXTURES)
    uncovered = [lv["id"] for lv in levers if lv["id"] not in covered]
    print()
    if uncovered:
        print("no self-test case for: %s" % ", ".join(uncovered))
    print("%d lever(s); %d failure(s); %d skipped"
          % (len(levers), bad, skipped))
    return 1 if bad else 0


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    flags = {a for a in sys.argv[1:] if a.startswith("--")}
    with open(LEVERS) as fh:
        levers = yaml.safe_load(fh)
    if "--self-test" in flags:
        return self_test(levers)
    if len(args) != 2:
        print((__doc__ or "").strip())
        return 2
    unit, func = args
    path = "%s/asm/nonmatchings/%s/%s.s" % (PROJ, unit, func)
    if not os.path.exists(path):
        # the listing is deleted for a function already in C
        alt = "%s/asm/nonmatchings/%s" % (PROJ, unit)
        if os.path.isdir(alt):
            print("no listing at %s" % path)
            print("(already decompiled? `make splat` regenerates nonmatchings)")
        else:
            print("no such unit: %s" % unit)
        return 2
    return report(unit, func, levers, parse_listing(path), "--sites" in flags)


if __name__ == "__main__":
    sys.exit(main())
