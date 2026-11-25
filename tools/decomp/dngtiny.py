#!/usr/bin/env python3
"""dngtiny -- the stubs dngpick CANNOT SEE, grouped by identical body.

docs/dng-twinsweep.json has no `words' field for a function below its minimum
size, which is what this tool keys on: 94 stubs and 937 words, 12% of the
remaining FUNCTIONS for under 1% of the remaining words.  The project metric
counts functions as well as words, so these are the cheapest rows on the board.

**THEY ARE NOT INVISIBLE TO `dngpick' AND I SAID THEY WERE.  They are
OUTRANKED.**  `dngfam' computes sizes for itself, so `dngpick.stub_rows()'
carries every one of them with the right word count -- and already knew the
thirteen-member family below, at `copies=13 yield=104'.  What keeps them out of
a batch is the RANKING: units are scored by total twin yield and packed against
a word budget, so a yield of 2, 3 or 104 never wins against a unit offering
800+.  The fix is therefore a ranking or a mode, NOT a new data source, and
this tool is a surfacing tool rather than a discovery.

They also replicate heavily.  Thirteen stubs in ovl_dng_b31f are the same
eight-word thunk byte for byte -- one spelling lands all thirteen -- so this
tool groups by normalised body text rather than listing them flat.

`dngpick' DOES have a real blind spot, and it is a different and smaller one:
21 stubs sit in no family at all and never reach `stub_rows()'.  Nineteen are
parks, which it skips deliberately.  The other is
**`func_lo20_8001E814', a 6,667-WORD STUB** -- the largest single function in
the remaining work, 5% of everything left, in no family, never offered, and
not parked.

    dngtiny.py                 every tiny stub, classes first
    dngtiny.py --unit b31f     one unit
    dngtiny.py --classes       only the multi-member classes
    dngtiny.py --max 24        raise the size cut (default: whatever the
                               sweep omitted, i.e. no cut at all)

A CLASS IS A PROMISE ABOUT THE SOURCE, NOT A GUARANTEE.  Identical asm means
one spelling compiles to all members, but each member still has its own
listing and must be verified on its own -- a bank-local symbol renamed per copy
is normalised here and is NOT normalised by the compiler.
"""

import argparse
import collections
import glob
import json
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dngfam  # noqa: E402

PRE = re.compile(r"^\s*/\* [0-9A-F]+ [0-9A-F]+ [0-9A-F]+ \*/\s{1,2}")
HDR = re.compile(r"nonmatching (func_[0-9A-Za-z_]+), (0x[0-9A-Fa-f]+)")


def _code(text):
    """The text from the function's glabel on, so a leading .rodata table's
    words are not counted as part of the body."""
    i = text.find("glabel func_")
    return text if i < 0 else text[i:]


def body(text):
    """Normalised instruction text: no addresses, no per-copy label or symbol.

    The prefix MUST be stripped line by line.  A regex that skips a line it
    cannot match (an operandless `nop') re-matches across the newline and
    swallows the next line's address comment, which makes every copy look
    distinct -- that is exactly how this tool's first version reported 92
    distinct bodies for 94 stubs, including thirteen that are byte-identical.
    """
    out = []
    for line in text.split("\n"):
        if not PRE.match(line):
            continue
        ins = PRE.sub("", line).rstrip()
        ins = re.sub(r"\.L[a-z0-9]+_[0-9A-F]+", ".L", ins)
        ins = re.sub(r"(func|D|jtbl)_[a-z0-9]*_?[0-9A-F]{6,8}", r"\1_X", ins)
        out.append(ins)
    return "\n".join(out)


def tiny(max_words=0):
    """[(words, func, unit, normalised body)] for stubs the sweep omits."""
    sweep = json.load(open(os.path.join(dngfam.PROJ, "docs/dng-twinsweep.json")))
    stubs = dngfam.stubs()
    rows = []
    for path in glob.glob(os.path.join(dngfam.PROJ, "asm/nonmatchings/*/*.s")):
        text = open(path, encoding="utf-8", errors="replace").read()
        # search the WHOLE text, not a prefix: a unit whose listing carries a
        # leading `.section .rodata' jump table has its `nonmatching func_...'
        # header well past any fixed window, and a prefix search silently
        # SKIPS those functions -- which is how this tool's first count of
        # the pool came in short.
        m = HDR.search(text)
        if not m:
            continue
        func = m.group(1)
        if func not in stubs:
            continue
        words = int(m.group(2), 16) // 4
        if not words:
            continue
        # the sweep's own omission is the definition, unless --max overrides
        if max_words:
            if words > max_words:
                continue
        elif sweep.get(func, {}).get("words"):
            continue
        unit = os.path.basename(os.path.dirname(path))
        rows.append((words, func, unit, body(_code(text))))
    return rows


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--unit", default="", help="restrict to one unit tag or name")
    ap.add_argument("--classes", action="store_true",
                    help="only the classes with more than one member")
    ap.add_argument("--max", type=int, default=0,
                    help="size cut in words (default: whatever the sweep omits)")
    args = ap.parse_args()

    rows = tiny(args.max)
    if args.unit:
        want = args.unit if args.unit.startswith("ovl_") else "ovl_dng_" + args.unit
        rows = [r for r in rows if r[2] == want]
        if not rows:
            print("no tiny stub in %s" % want)
            return

    cls = collections.defaultdict(list)
    for words, func, unit, b in rows:
        cls[b].append((func, words, unit))
    groups = sorted(cls.values(), key=lambda v: (-len(v), -v[0][1]))

    multi = [g for g in groups if len(g) > 1]
    print("%d tiny stub(s), %d words, %d distinct body(s)"
          % (len(rows), sum(r[0] for r in rows), len(groups)))
    if multi:
        print("\n%d CLASS(ES) WITH MORE THAN ONE MEMBER -- one spelling each:"
              % len(multi))
        for g in multi:
            g = sorted(g)
            print("  %2d x %2dw = %3dw" % (len(g), g[0][1], sum(x[1] for x in g)))
            for func, words, unit in g:
                print("        %-28s %s" % (func, unit))
    if args.classes:
        return

    singles = [g[0] for g in groups if len(g) == 1]
    if singles:
        print("\n%d singleton(s), %d words:" % (len(singles), sum(s[1] for s in singles)))
        per = collections.defaultdict(list)
        for func, words, unit in singles:
            per[unit].append((words, func))
        for unit in sorted(per, key=lambda u: -len(per[u])):
            v = sorted(per[unit])
            print("  %-22s %2d fn %4dw" % (unit, len(v), sum(w for w, _ in v)))
            for words, func in v:
                print("        %-28s %3dw" % (func, words))


if __name__ == "__main__":
    main()
