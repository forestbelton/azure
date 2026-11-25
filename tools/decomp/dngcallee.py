#!/usr/bin/env python3
"""dngcallee.py -- donors by CALLEE-SET overlap, per FUNCTION not per file.

## WHY THIS EXISTS

It is the recipe that has landed every naked body in the last three batches,
and until now every agent ran it by hand. `dngtwin`'s ratio cannot reach this
class at all, and NOT for the reason the agents guessed.

Measured on `func_b01_80172A48` (531 words), which `dngtwin` reports as having
no sibling at any floor while FOURTEEN landed relinks of it exist:

    func_b27_801737C8   425w   IN the length band, ratio 0.546 -- under the floor
    func_b55_8017256C   376w   outside the band
    func_b20_80172BCC   325w   outside the band
    func_b28_80172A58   286w   outside the band
    func_b40_80170EA8   356w   outside the band

Every one is LANDED and every one is in `dngtwin`'s pool -- landed bodies enter
it from the built objects, so a deleted `asm/nonmatchings/` listing is NOT the
cause (an agent proposed that and it is wrong). The cause is that these are
`+0x9B` STATE MACHINES WHOSE ARM COUNT DIFFERS PER BANK. The bodies really are
531 against 425 against 286 words, so the word count diverges and mnemonic
similarity diverges with it -- while the CALLEE SET stays almost invariant,
because every bank calls the same library to do the same job.

**So length-banded mnemonic similarity is the wrong metric for this family, and
no floor fixes it.** This is.

## THE ONE THING THAT MAKES IT WORK

Intersecting FILES is useless: on that body the eight symbols appeared in
fifty-odd files. What works is to split each unit into FUNCTIONS and count how
many of the symbols each function BODY contains. That put all fourteen relinks
at 8/8 and nothing else above 4 -- a clean separation, not a ranking.
"""
import collections, os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dngclone

PROJ = "/home/case/projects/azure"
SYM = re.compile(r"\b(?:func|D)_[A-Za-z0-9]*_?[0-9A-Fa-f]{6,8}\b")
JAL = re.compile(r"\bjal\s+(func_\w+)")
DATA = re.compile(r"%(?:hi|lo)\((D_\w+)\)")
# THE TAG SEGMENT IS OPTIONAL. This REQUIRED one -- `func_<tag>_<8 hex>' --
# so it matched no RESIDENT function (`func_800B8830' has no tag) and parsed
# ZERO definitions out of all 60 non-dng units. That is the exact mirror of the
# bug fixed in permimport.py the same hour, where a pattern FORBADE the tag and
# so matched no carved name. Both silently exclude half the corpus and neither
# errors: write `(?:[A-Za-z][A-Za-z0-9]*_)?' and cover both.
DEFN = re.compile(r"^[A-Za-z_][A-Za-z0-9_ \t*]*?\b(func_(?:[A-Za-z][A-Za-z0-9]*_)?"
                  r"[0-9A-Fa-f]{8})\s*\([^;]*$")
STUB = re.compile(r'INCLUDE_ASM\("[^"]*",\s*(func_\w+)\)')


def wanted(func):
    """The symbol set to search for: this stub's jal targets and data refs."""
    m = re.match(r"^func_([A-Za-z][A-Za-z0-9]*)_", func)
    if not m:
        return None
    p = os.path.join(PROJ, "asm", "nonmatchings", "ovl_dng_" + m.group(1),
                     func + ".s")
    if not os.path.exists(p):
        return None
    t = open(p, errors="replace").read()
    return set(JAL.findall(t)) | set(DATA.findall(t))


def bodies():
    """-> [(unit, func, symbolset)] for every LANDED body in every dng unit."""
    out = []
    import glob
    # SEARCH THE RESIDENT UNITS TOO, not just the carved dng ones.
    #
    # This globbed `ovl_dng_*.c' only. An agent in batch 31 landed a 271-word
    # body off the RESIDENT `func_800B8830' in csrc/ovl_dungeon_800B6AC4.c,
    # found by running the intersection BY HAND over the whole of csrc/ --
    # and reported that neither this tool's rows nor dngnear's were what did
    # it. Of course not: the donor was in one of the 60 non-dng units this
    # never opened.
    #
    # That is the obvious half of the recipe and I shipped the narrow half. The
    # carved block calls the resident library constantly, so a resident body
    # that uses the same four or five library symbols is exactly as good a
    # donor as a banked sibling -- better, often, because the resident copy is
    # the ORIGINAL the banks were relinked from.
    for path in sorted(glob.glob(os.path.join(PROJ, "csrc", "*.c"))):
        unit = os.path.basename(path)[:-2]
        raw = open(path, errors="replace").read()
        stubs = set(STUB.findall(raw))
        # STRIP COMMENTS BEFORE ATTRIBUTING ANY SYMBOL.
        #
        # A landed body's comment sits BETWEEN the previous function's closing
        # brace and its own definition line, so a line-by-line walk credits
        # every comment to the PRECEDING function -- and these comments are
        # dense with symbol names (twins, tables, data). Measured: the 9-line
        # forwarder func_b03_801725BC, which references THREE symbols, was
        # credited with EIGHTEEN, and an agent noticed because the tool offered
        # it as a 4-of-5 donor when the function that really matches is the one
        # below it. Every row this tool has ever printed was inflated this way.
        #
        # dngclone documents the same trap for the same reason ("the search
        # runs on the body's CODE, not its comment" -- 32 wrong declarations
        # across 28 units), and its `code_mask' is the one parser for it.
        txt = "".join(c for c, keep in zip(raw, dngclone.code_mask(raw)) if keep)
        cur, acc = None, set()
        for ln in txt.split("\n"):
            m = DEFN.match(ln)
            if m and m.group(1) not in stubs:
                if cur:
                    out.append((unit, cur, acc))
                cur, acc = m.group(1), set()
                continue
            if cur:
                acc |= set(SYM.findall(ln))
        if cur:
            out.append((unit, cur, acc))
    return out


def main():
    # --top's VALUE must not fall through into the function list. It did, and
    # the tool printed a row for "6". Same bug I had just fixed in dngnear.py,
    # written the same wrong way twice in one day -- hence the shared shape.
    args, top, i = sys.argv[1:], 12, 0
    argv = []
    while i < len(args):
        if args[i] == "--top" and i + 1 < len(args):
            top = int(args[i + 1])
            i += 2
            continue
        if not args[i].startswith("-"):
            argv.append(args[i])
        i += 1
    if not argv:
        raise SystemExit("usage: dngcallee.py FUNC [FUNC ...] [--top N]")
    bl = bodies()
    for f in argv:
        w = wanted(f)
        print("\n%s" % f)
        if w is None:
            print("    no listing -- landed already, or not a carved function")
            continue
        w = {s for s in w if not s.startswith("func_%s_"
                                              % re.match(r"func_([A-Za-z0-9]*)_", f).group(1))}
        if not w:
            print("    no non-local callees or data refs to intersect")
            continue
        print("    searching %d symbol(s): %s"
              % (len(w), " ".join(sorted(w))))
        rows = []
        for unit, fn, acc in bl:
            if fn == f:
                continue
            k = len(w & acc)
            if k:
                rows.append((k, unit, fn))
        rows.sort(reverse=True)
        if not rows:
            print("    NOTHING shares a symbol -- this body is genuinely alone")
            continue
        # The separation matters more than the ranking: report the best score
        # and everything at it, then say how far the next tier is.
        best = rows[0][0]
        at_best = [r for r in rows if r[0] == best]
        print("    %d of %d symbols is the best overlap, reached by %d "
              "function(s):" % (best, len(w), len(at_best)))
        for k, unit, fn in at_best[:top]:
            print("        %-24s csrc/%s.c" % (fn, unit))
        nxt = [r for r in rows if r[0] < best]
        if nxt:
            print("    next tier is %d/%d (%d function(s)) -- a CLEAN GAP means "
                  "the rows above are the family" % (nxt[0][0], len(w),
                                                     sum(1 for r in nxt if r[0] == nxt[0][0])))


if __name__ == "__main__":
    main()
