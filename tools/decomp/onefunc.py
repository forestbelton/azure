#!/usr/bin/env python3
"""onefunc.py FUNC -- build a STANDALONE one-function copy of a unit, so that
`aligndiff`/`ovlverify` cost 0.09 s a candidate instead of 25 s.

    python3 tools/decomp/onefunc.py func_800A08A0 --src /tmp/mine/ovl_dungeon.c
    -> /tmp/mine/one/ovl_dungeon.c          (then measure THAT path)

The file is the unit's own top declaration block -- everything before its
first function definition or INCLUDE_ASM -- plus the one function's
definition, and nothing else. That matters twice over:

* the declarations are the UNIT'S, not hand-written ones, which is the failure
  docs/town-sweep.md records as "AN AGENT'S STANDALONE FILE CAN DECLARE A
  SYMBOL DIFFERENTLY THAN csrc/ DOES, so its 0 differing does not transfer"
  (`func_8008FFC8`: 146 words standalone against 152 in the unit, on one
  `extern u8 x[]` that the unit declares as a struct);
* the basename stays `ovl_dungeon.c`, because `ovlverify.py` picks which
  container ELF to resolve landed callees against FROM THE BASENAME. A copy
  called anything else falls back to build/town.bin.elf and prints plausible
  nonsense. The output therefore goes in a SUBDIRECTORY, same name.

WHY THIS IS A MEASUREMENT AND NOT A SHORTCUT: 37k lines of unit compile in
~25 s, and that cost is paid per candidate. On func_800A08A0 the standalone
turned a scheduling residue from "a few spellings an hour" into a sweep of
twenty in a minute, which is what made reading the .sched dump worth doing at
all -- tools/decomp/dump.sh on the whole unit is impractical, on this file it
is instant.

THE ONE WAY IT LIES, AND THE TOOL CHECKS FOR IT
-----------------------------------------------
A callee DEFINED EARLIER in the unit with no prototype is seen by the real
compile as a declared function and by this file as an implicit `int f()`. The
difference is real -- it is worth two words on func_8009DAD8's call to
func_8009FD40 (a lost `sll 16`/`sra 16` pair before a `slti`) -- so the tool
reports every callee of this function that is NOT declared in the preamble,
splitting them into

    DEFINED EARLIER  -- the standalone MAY disagree with the unit; confirm
                        the baseline before trusting a sweep
    defined later / elsewhere -- implicit in both, so harmless

It is a "check this", not a verdict: it fires whenever the two compiles COULD
differ, and often they do not.  func_8009DAD8 calls func_8009DA70, which is
defined earlier with no prototype -- the warning fires and the standalone
still agrees with the unit at `0 differing`, because that callee's arguments
are already the promoted types and its result is unused.  It is worth two
words when it does bite (func_8009FD40, a lost `sll 16`/`sra 16` pair before
a `slti`), which is why the check exists and why confirming the baseline is
not optional.

ALWAYS CONFIRM THE BASELINE BEFORE TRUSTING A SWEEP, and re-measure the final
body against the real unit before landing it. Both were done for every
function landed through this tool: the standalone and the unit agreed on the
park's stated residue first, and on `0 differing` last.

`ovlverify` on a standalone file counts the unresolved `jal` targets of
functions that are not in it, so its masked count is NOT comparable --
compare `aligndiff`'s row count, and run `ovlverify` against the real unit.
"""
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.environ.get("AZURE_ROOT", os.path.dirname(os.path.dirname(HERE)))


def find_definition(text, func):
    """Return (start, end) of func's definition, or None.

    Anchors on a parameter list followed by `{`, which is what rejects the
    FORWARD DECLARATION -- the trap docs/dungeon-parks/README.md records as
    eating ~37000 lines when a park was spliced with a regex. A park's `#if 0`
    alternative body is a second definition; the LIVE one is the first.
    """
    out = []
    for m in re.finditer(r"(?m)^[A-Za-z_][^\n;]*\b" + re.escape(func) + r"\s*\(", text):
        k = m.end() - 1
        depth = 0
        while k < len(text):
            if text[k] == "(":
                depth += 1
            elif text[k] == ")":
                depth -= 1
                if depth == 0:
                    break
            k += 1
        rest = text[k + 1:]
        stripped = rest.lstrip()
        if not stripped.startswith("{"):
            continue
        i = k + 1 + (len(rest) - len(stripped))
        depth = 0
        j = i
        while j < len(text):
            if text[j] == "{":
                depth += 1
            elif text[j] == "}":
                depth -= 1
                if depth == 0:
                    break
            j += 1
        out.append((m.start(), j + 1))
    return out[0] if out else None


def preamble_end(text):
    """Index one past the unit's declaration block."""
    lines = text.split("\n")
    for i, l in enumerate(lines):
        if l.startswith("INCLUDE_ASM("):
            return i
        if i > 20 and re.match(r"^[A-Za-z_][^;]*\)\s*$", l) \
                and re.search(r"\bfunc_[0-9A-Fa-f]+\s*\(", l):
            return i
    raise SystemExit("onefunc: no function definition or INCLUDE_ASM found")


def main():
    args = sys.argv[1:]
    if not args or args[0].startswith("-"):
        raise SystemExit(__doc__)
    func = args[0]
    src = os.path.join(PROJ, "csrc/ovl_dungeon.c")
    out_dir = None
    i = 1
    while i < len(args):
        if args[i] == "--src":
            src = args[i + 1]
            i += 2
        elif args[i] == "--out":
            out_dir = args[i + 1]
            i += 2
        else:
            raise SystemExit("onefunc: unknown argument %r" % args[i])

    text = open(src).read()
    span = find_definition(text, func)
    if span is None:
        raise SystemExit("onefunc: no DEFINITION of %s in %s (still INCLUDE_ASM? "
                         "splice a park body in first with parksplice.py)" % (func, src))
    start, end = span
    body = text[start:end]
    pre = "\n".join(text.split("\n")[:preamble_end(text)])

    # Which callees does the preamble NOT declare?
    # Any mention, not just a call: an address-taken handler like
    # `n->unk_10 = (u32)func_800B14FC;` is an undeclared identifier too, and
    # catching only `func_X(` left the standalone failing to compile at all.
    called = sorted(set(re.findall(r"\b(func_[0-9A-Fa-f]{8})\b", body)) - {func})
    undeclared_earlier, undeclared_other = [], []
    for c in called:
        if re.search(r"(?m)^[A-Za-z_][^\n;]*\b" + c + r"\s*\([^;]*;", pre):
            continue
        d = find_definition(text, c)
        (undeclared_earlier if d and d[0] < start else undeclared_other).append(c)

    # A callee DEFINED EARLIER with no prototype is a DECLARED function to the
    # real compile and an implicit `int f()' to this file, and that difference
    # is worth two words when it bites. Reproduce the unit's view by
    # synthesising a prototype from the callee's own definition line -- which
    # is exactly what the unit's compile sees, so it ADDS no divergence.
    # Without this the standalone is unusable for any function whose callees
    # sit above it, which on ovl_dungeon.c is most of the back half.
    synth = []
    for c in list(undeclared_earlier):
        d = find_definition(text, c)
        head = text[d[0]:text.index("{", d[0])].strip().rstrip()
        head = " ".join(head.split())
        if head.endswith(")"):
            synth.append(head + ";")
            undeclared_earlier.remove(c)
    if synth:
        pre = pre + "\n\n/* onefunc.py: prototypes for callees DEFINED EARLIER in the unit with no\n" \
                    "   declaration of their own -- the real compile sees these, so the\n" \
                    "   standalone file has to as well. */\n" + "\n".join(synth) + "\n"

    if out_dir is None:
        out_dir = os.path.join(os.path.dirname(os.path.abspath(src)), "one")
    os.makedirs(out_dir, exist_ok=True)
    dst = os.path.join(out_dir, os.path.basename(src))
    with open(dst, "w") as f:
        f.write(pre + "\n\n" + body + "\n")

    # The unit's declaration block now lives in a HEADER (csrc/ovl_dungeon.c
    # was carved on 2026-11-20, so both units share it). gcc resolves a
    # quoted include against the INCLUDING FILE'S directory first, and the
    # standalone file is not in csrc/ -- so copy every header the preamble
    # includes by quoted name next to it, or the file compiles with no
    # declarations at all and every measurement is nonsense.
    copied = []
    for h in re.findall(r'(?m)^\s*#\s*include\s+"([^"]+)"', pre):
        cand = os.path.join(os.path.dirname(os.path.abspath(src)), h)
        if not os.path.exists(cand):
            cand = os.path.join(PROJ, "csrc", h)
        if os.path.exists(cand) and os.path.abspath(cand) != os.path.abspath(
                os.path.join(out_dir, h)):
            tgt = os.path.join(out_dir, h)
            os.makedirs(os.path.dirname(tgt) or ".", exist_ok=True)
            with open(cand) as fh, open(tgt, "w") as fo:
                fo.write(fh.read())
            copied.append(h)

    print("onefunc: %s -> %s (%d preamble lines + %d body lines)"
          % (func, dst, pre.count("\n") + 1, body.count("\n") + 1))
    if copied:
        print("onefunc: copied alongside it: %s" % " ".join(copied))
    if synth:
        print("onefunc: synthesised %d prototype(s) for callees defined earlier in the"
              % len(synth))
        print("onefunc: unit with no declaration, so this file sees what the unit sees:")
        for h in synth:
            print("onefunc:       %s" % h)
    if undeclared_earlier:
        print("onefunc: *** MAY DISAGREE WITH THE UNIT *** these callees are DEFINED")
        print("onefunc: *** EARLIER in %s with no prototype, so the unit sees a" % os.path.basename(src))
        print("onefunc: *** declaration and this file sees an implicit `int f()'.")
        print("onefunc: *** Often it makes no difference; CONFIRM THE BASELINE:")
        for c in undeclared_earlier:
            print("onefunc:       %s" % c)
        print("onefunc: *** if this file reproduces the residue you measured in the")
        print("onefunc: *** unit, it is safe to sweep on; if it does not, add the")
        print("onefunc: *** prototype to the unit rather than to this file.")
    if undeclared_other:
        print("onefunc: implicit in BOTH (defined later or in another unit), harmless: %s"
              % " ".join(undeclared_other))
    print("onefunc: CONFIRM THE BASELINE -- measure this file and the real unit and")
    print("onefunc: check they agree, before sweeping anything. Re-measure the final")
    print("onefunc: body against the real unit before landing, and use ovlverify")
    print("onefunc: THERE: on a standalone file its masked count includes the")
    print("onefunc: unresolved `jal' targets of every function that is not in it.")


if __name__ == "__main__":
    main()
