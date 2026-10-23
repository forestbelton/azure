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
    """Check every lever's signature against a function known to need it."""
    cases = [
        ("unk_102A8", "func_8003D0F0", "byte-fold"),
        ("unk_14934", "func_80042A80", "srl-not-sra"),
        ("unk_14934", "func_80042560", "block-move"),
        ("unk_12DE0", "func_8003F5E0", "gp-relative"),
        ("unk_14934", "func_80042BDC", "constant-vs-symbol"),
        ("unk_14934", "func_80042BDC", "switch-index-width"),
        ("unk_14934", "func_80041344", "loop-guard-blez"),
    ]
    bad = 0
    for unit, func, want in cases:
        path = "%s/asm/nonmatchings/%s/%s.s" % (PROJ, unit, func)
        if not os.path.exists(path):
            print("SKIP %-14s %-14s (no listing; run make splat)" % (func, want))
            continue
        ids = [lv["id"] for lv, _ in applicable(levers, parse_listing(path))]
        ok = want in ids
        bad += 0 if ok else 1
        print("%-4s %-14s expects %-20s got: %s"
              % ("ok" if ok else "FAIL", func, want, ", ".join(ids) or "-"))
    print()
    print("%d lever(s) in levers.yaml; %d self-test failure(s)" % (len(levers), bad))
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
