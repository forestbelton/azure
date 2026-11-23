#!/usr/bin/env python3
"""Give each carved DUNGEON.BIN object its own `.rodata` subsegment.

WHY. A carved segment's leading subsegment is declared `data`, and for most of
them that is wrong in a way that only shows up when a function lands. The
bytes are not a data blob: after a short prefix of real data they are the
unit's COMPILER-GENERATED JUMP TABLES. While every function is `INCLUDE_ASM`
splat migrates each table into its own function's listing and emits it
verbatim, so the container is byte-identical and nothing is amiss. The moment
a function with a `switch` lands, cc1psx emits its own table into `.rodata`,
the linker appends it past the code, and the container grows by the table's
size for every landed member of that function's family.

Batch 5 hit this on `func_b03_80170E54`: six members, 0x30 bytes each,
dungeon.bin 0x120 too long, the built code loading its own table at
0x80173C80 where the ROM loads `jtbl_b03_80170808`. `ovlverify` reported
`0 differing` for all six, because it measures a function's own words against
its listing and never looks at `.rodata`. Only the container build knows.

WHAT THIS DOES. For each segment that owns at least one jump table, insert

    - [<first table's file offset>, .rodata, <unit>]

between the leading `data` subsegment and the `c` subsegment -- or convert the
leading `data` outright when the first table is at the segment start. The
tables then land at their ROM addresses whether they come from a landed body
or from an `INCLUDE_ASM` listing.

`.rodata` WITH THE DOT, because the sibling C file exists. splat refuses the
undotted form for a segment that has one, and without a sibling the dot
extracts nothing and every reference into the overlay's own rodata comes out
undefined (docs/overlays.md, "The rodata dot").

THE ALIGNMENT RULE, which is what makes this cheap here and expensive in the
resident overlay: cc1psx writes `.align 3` before every table IT compiles, so
a table at a 4-mod-8 offset RELATIVE TO ITS UNIT'S RODATA START gains four
bytes of pad the reference does not have. All 106 carved segments have their
first table at 0 mod 8 absolutely, so no `subalign` is needed, and in 104 of
them every other table is 0 mod 8 relative to the first as well. The two
exceptions are reported by name: splitting them needs a matching `c` split at
the owning function, because cc1psx emits a unit's rodata in FUNCTION ORDER
and the two boundaries must name the same function. Until that function
lands, its table comes from asm with no `.align` and costs nothing, so this
tool leaves those two alone and says so.

Usage:
    python3 tools/decomp/dngrodata.py            # analyse, change nothing
    python3 tools/decomp/dngrodata.py --apply    # edit splat/dungeon.bin.yaml
"""
import argparse
import glob
import os
import re
import sys

PROJ = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
YAML = os.path.join(PROJ, "splat", "dungeon.bin.yaml")

SEG = re.compile(r"^  - name: (ovl_dng_\w+)\n((?:    [^\n]*\n|      [^\n]*\n)+)", re.M)
SUB = re.compile(r"^      - \[(0x[0-9a-fA-F]+), ([.\w]+)(?:, (\w+))?\]", re.M)


def segments(text):
    out = []
    for m in SEG.finditer(text):
        blk = m.group(2)
        st = re.search(r"^    start: (0x[0-9a-fA-F]+)", blk, re.M)
        vr = re.search(r"^    vram: (0x[0-9a-fA-F]+)", blk, re.M)
        if st and vr:
            out.append({"name": m.group(1), "start": int(st.group(1), 16),
                        "vram": int(vr.group(1), 16),
                        "subs": SUB.findall(blk), "span": m.span(),
                        "block": blk})
    return out


ROM = os.path.join(PROJ, "data", "dungeon", "dungeon.bin")
_rom = None


def rom():
    global _rom
    if _rom is None:
        _rom = open(ROM, "rb").read()
    return _rom


def tables(unit):
    """{vram} of every jump table `unit`'s listings REFERENCE.

    By reference, not by `dlabel`. A table only gets a `dlabel` in its
    function's listing once the segment HAS a `.rodata` subsegment -- which is
    the thing this tool is adding -- so reading dlabels finds nothing on an
    unsplit segment and silently reports that it owns no tables. A first cut
    of this did exactly that and its contiguity check passed vacuously on all
    116 segments.
    """
    out = set()
    for f in glob.glob(os.path.join(PROJ, "asm", "nonmatchings", unit, "*.s")):
        for v in re.findall(r"jtbl_\w+_([0-9A-Fa-f]{8})", open(f).read()):
            out.add(int(v, 16))
    return out


def all_entries_are_code(off, end, vram, start, code_vram, text_end):
    """True if every word in [off, end) is an address in this unit's code.

    The check that the region really is nothing but jump tables, made against
    the ROM rather than against splat's output so it holds before the split.
    A table's entries are addresses inside its own function, so every word in
    a run of tables lies in the segment's text. Anything else there -- a
    string, a float pool, a const array -- fails this and the segment is left
    for a human.
    """
    b = rom()
    for p in range(off, end, 4):
        w = int.from_bytes(b[p:p + 4], "little")
        if not (code_vram <= w < text_end):
            return False, p, w
    return True, None, None


def plan(text):
    """[(seg, first table file offset, [4-mod-8 offenders], n tables)]."""
    todo, skip = [], []
    for s in segments(text):
        if any(t == ".rodata" for _, t, _ in s["subs"]):
            skip.append((s["name"], "already has a .rodata subsegment"))
            continue
        tb = tables(s["name"])
        if not tb:
            skip.append((s["name"], "owns no jump table"))
            continue
        code = [int(a, 16) for a, t, _u in s["subs"] if t == "c"]
        if not code:
            skip.append((s["name"], "has no c subsegment"))
            continue
        code = code[0]
        # The segment's text runs from the c subsegment to the next
        # subsegment, or to the end of the segment.
        later = sorted(int(a, 16) for a, _t, _u in s["subs"]
                       if int(a, 16) > code)
        text_end_off = later[0] if later else None
        first = min(tb)
        off = first - s["vram"] + s["start"]
        if off < s["start"] or off >= code:
            skip.append((s["name"], "first table %#x is not inside the "
                                    "leading subsegment" % first))
            continue
        code_vram = code - s["start"] + s["vram"]
        text_end = ((text_end_off - s["start"] + s["vram"]) if text_end_off
                    else code_vram + 0x40000)
        ok, at, w = all_entries_are_code(off, code, s["vram"], s["start"],
                                         code_vram, text_end)
        if not ok:
            skip.append((s["name"], "%#x in the proposed .rodata is %#010x, "
                                    "not an address in this unit's code -- "
                                    "needs reading by hand" % (at, w)))
            continue
        bad = sorted(v for v in tb if (v - first) % 8)
        todo.append((s, off, bad, len(tb)))
    return todo, skip


def apply(text, todo):
    """Insert each `.rodata` line. Offsets shift, so edit back to front."""
    n = 0
    for s, off, bad, _ntb in sorted(todo, key=lambda x: -x[0]["span"][0]):
        if bad:
            continue
        lead = [(a, t, u) for a, t, u in s["subs"]][0]
        line = "      - [%#x, .rodata, %s]\n" % (off, s["name"])
        blk = s["block"]
        if int(lead[0], 16) == off and lead[1] == "data":
            # The first table IS the segment start, so the whole leading
            # subsegment is rodata and no data prefix remains.
            old = "      - [%s, data]\n" % lead[0]
            new = line
        else:
            old = "      - [%s, %s]\n" % (lead[0], lead[1])
            new = old + line
        if old not in blk:
            raise SystemExit("%s: leading subsegment line not found" % s["name"])
        newblk = blk.replace(old, new, 1)
        text = text[:s["span"][0]] + text[s["span"][0]:s["span"][1]].replace(
            blk, newblk, 1) + text[s["span"][1]:]
        n += 1
    return text, n


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--apply", action="store_true")
    a = ap.parse_args()
    text = open(YAML).read()
    todo, skip = plan(text)
    print("%-16s %5s %12s  %s" % ("unit", "tbls", "rodata at", "note"))
    for s, off, bad, ntb in todo:
        note = ("SKIPPED: 4-mod-8 table(s) %s need a matching c split first"
                % ",".join("%#x" % v for v in bad)) if bad else ""
        print("%-16s %5d %12s  %s" % (s["name"], ntb, hex(off), note))
    held = [s["name"] for s, _o, bad, _n in todo if bad]
    print("\n%d segment(s) to split, %d held back for alignment%s"
          % (len([1 for _s, _o, b, _n in todo if not b]), len(held),
             (": " + ", ".join(held)) if held else ""))
    for name, why in skip:
        if "already" not in why and "owns no" not in why:
            print("  NOTE %s: %s" % (name, why))
    if not a.apply:
        print("(analysis only; pass --apply to edit %s)"
              % os.path.relpath(YAML, PROJ))
        return
    text, n = apply(text, todo)
    open(YAML, "w").write(text)
    print("wrote %s: %d .rodata subsegment(s) added"
          % (os.path.relpath(YAML, PROJ), n))


if __name__ == "__main__":
    main()
