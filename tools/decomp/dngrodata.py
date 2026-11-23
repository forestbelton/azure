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


def splat_emits(unit, sym):
    """True if splat already defines `sym` in one of the unit's listings.

    MUST BE ASKED AFTER THE SPLIT. spimdisasm writes a rodata symbol into a
    function's listing when it considers the function to OWN it, and whether
    it does cannot be known before the subsegment is `.rodata'. Four of the
    47 blobs turned out to be owned that way -- D_s18_80024074,
    D_s37_80024050, D_s40_80024038, D_s42_80024064 -- and writing a `const'
    array for one of those is a DOUBLE definition, which the assembler
    reports as `symbol D_s18_80024074 is already defined' from a line in
    include/labels.inc rather than from anything a person wrote.
    """
    for f in glob.glob(os.path.join(PROJ, "asm", "nonmatchings", unit, "*.s")):
        if re.search(r"dlabel %s\b" % re.escape(sym), open(f).read()):
            return True
    return False


def data_refs(unit, lo, hi):
    """{sym: {function}} for every D_* symbol the unit references in [lo, hi).

    THIS, NOT A BYTE HEURISTIC, IS THE TEST. A first cut asked whether every
    word in the proposed `.rodata` was an address inside the unit's own code,
    and rejected 53 segments. 31 of those were false: the offending word was
    0x00000000, a NULL entry for an unreachable case INSIDE a jump table, and
    a run of tables is perfectly free to contain one.

    What actually blocks a segment is a data symbol in the region that the
    CODE REFERENCES. splat creates `D_<tag>_<vram>` only where something
    points at it, migration to a text subsegment is off for a rodata
    subsegment with a sibling C file, and a symbol referenced by a function
    but not owned by it is then emitted nowhere -- so the link fails with
    `undefined reference'. Asking which symbols are referenced answers exactly
    the question the linker will ask.
    """
    out = {}
    for f in glob.glob(os.path.join(PROJ, "asm", "nonmatchings", unit, "*.s")):
        txt = open(f).read()
        for m in re.finditer(r"%[hl][io]\((D_\w+_([0-9A-Fa-f]{8}))\)", txt):
            v = int(m.group(2), 16)
            if lo <= v < hi:
                out.setdefault(m.group(1), set()).add(
                    os.path.basename(f)[:-2])
    return out


def region_words(off, end, vram, start, code_vram, text_end):
    """(ok, offset, word) -- every word is a table entry or a NULL entry.

    Kept as a SECOND check behind data_refs, because an unreferenced blob is
    still a blob: nothing would define it either, it just fails later and
    less clearly. A word here must be an address in this unit's code, or zero.
    """
    b = rom()
    for p in range(off, end, 4):
        w = int.from_bytes(b[p:p + 4], "little")
        if w != 0 and not (code_vram <= w < text_end):
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
        refs = data_refs(s["name"], first, code_vram)
        if refs:
            skip.append((s["name"], "%d referenced rodata symbol(s) in the "
                                    "region -- needs const C data first: %s"
                         % (len(refs), ", ".join(sorted(refs)))))
            continue
        ok, at, w = region_words(off, code, s["vram"], s["start"],
                                 code_vram, text_end)
        if not ok:
            skip.append((s["name"], "%#x in the proposed .rodata is %#010x, "
                                    "neither an address in this unit's code "
                                    "nor zero -- needs reading by hand"
                         % (at, w)))
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



def table_owner(unit, vram):
    """The function whose listing carries the jump table at `vram`."""
    want = "_%08X" % vram
    for f in glob.glob(os.path.join(PROJ, "asm", "nonmatchings", unit, "*.s")):
        txt = open(f).read()
        if re.search(r"jtbl_\w+%s\b" % want, txt, re.I):
            return os.path.basename(f)[:-2]
    return None


def blob_plan(seg, lo, hi, tbl, refs):
    """[(sym, vram, size, anchor function, before?)] in address order.

    The ORDER MATTERS AND IT IS THE FUNCTION ORDER. cc1psx emits a unit's
    rodata in the order it compiles, so a `const' array declared between two
    functions lands between their jump tables -- which is exactly where these
    blobs sit in the ROM. So each blob is anchored to the owner of the NEXT
    table after it and inserted before that function; a blob after the last
    table is inserted after the owner of the one before it.
    """
    syms = {int(k.split("_")[-1], 16): k for k in refs}
    bounds = sorted(set(list(tbl) + list(syms) + [hi]))
    out = []
    for v in sorted(syms):
        end = next(b for b in bounds if b > v)
        later = [t for t in sorted(tbl) if t > v]
        if later:
            out.append((syms[v], v, end - v, table_owner(seg["name"], later[0]),
                        True))
        else:
            earlier = [t for t in sorted(tbl) if t < v]
            out.append((syms[v], v, end - v,
                        table_owner(seg["name"], earlier[-1]), False))
    return out


def const_text(sym, off, size, readers):
    b = rom()[off:off + size]
    rows = "\n".join("    " + ", ".join("0x%02X" % c for c in b[i:i + 8]) + ","
                      for i in range(0, size, 8))
    return ("/* %#x bytes of this bank's own rodata, between its jump tables,\n"
            "   read by %s. Spelled as BYTES because that is all the\n"
            "   reference witnesses, and `const' because without it the array\n"
            "   goes to .data and the linker puts it outside this segment's\n"
            "   declared .rodata. */\n"
            "const u8 %s[] = {\n%s\n};\n"
            % (size, ", ".join(sorted(readers)), sym, rows))


def anchor_span(text, func):
    """(start, end) of `func`'s stub line or its definition, in `text`."""
    m = re.search(r'(?m)^INCLUDE_ASM\("[^"]*",\s*%s\);[ \t]*$'
                  % re.escape(func), text)
    if m:
        return m.span()
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import dngclone
    body, span = dngclone.definition(text, func)
    if body is None:
        raise SystemExit("%s: no stub and no definition to anchor on" % func)
    return span


def write_consts(seg, plan):
    """Insert every blob's `const` array into the unit, and fix declarations."""
    path = os.path.join(PROJ, "csrc", seg["name"] + ".c")
    text = open(path).read()
    # A LANDED body in this unit may already declare the symbol
    # `extern u8 D_x[]'. Defining it `const u8' then conflicts, so the
    # declaration is corrected in the same pass -- and the caller re-verifies
    # that unit's functions, because changing a declaration can move code.
    fixed = []
    for sym, _v, _n, _a, _b in plan:
        pat = re.compile(r"(?m)^extern\s+(?:const\s+)?u8\s+%s\[\];[ \t]*$"
                         % re.escape(sym))
        if pat.search(text):
            text = pat.sub("extern const u8 %s[];" % sym, text)
            fixed.append(sym)
    # ONE INSERT PER ANCHOR, THE BLOBS INSIDE IT IN ASCENDING ADDRESS ORDER.
    # Inserting them one at a time reverses any group that shares an anchor:
    # each `before' insert lands immediately above the anchor, so the LAST
    # one written ends up closest to it. b10's two blobs went in as 0x874
    # then 0x854 and the container came out the right size with 0x185 bytes
    # of rodata swapped -- the kind of failure that looks like wrong CONTENT
    # and is really wrong ORDER.
    groups = {}
    for sym, v, n, anchor, before in plan:
        off = v - seg["vram"] + seg["start"]
        groups.setdefault((anchor, before), []).append(
            (v, const_text(sym, off, n, _READERS[sym])))
    ins = []
    for (anchor, before), items in groups.items():
        items.sort()
        ins.append((anchor, before, "\n".join(t for _v, t in items),
                    min(v for v, _t in items)))
    # Back to front across DIFFERENT anchors, so an earlier span stays valid.
    for anchor, before, body, _v in sorted(ins, key=lambda x: -x[3]):
        a, b = anchor_span(text, anchor)
        if before:
            text = text[:a] + body + "\n" + text[a:]
        else:
            text = text[:b] + "\n\n" + body + text[b:]
    open(path, "w").write(text)
    return sum(len(v) for v in groups.values()), fixed


_READERS = {}


def prune(only=None):
    """Remove every `const` array that duplicates a symbol splat now emits.

    A SEPARATE PASS, BECAUSE THE QUESTION CANNOT BE ASKED EARLIER. Whether
    spimdisasm writes a rodata symbol into its function's listing depends on
    the subsegment being `.rodata', so `--const' runs while the answer is
    still no for every symbol. Run `--const', re-run splat, then run this.
    Leaving a duplicate in place fails the assemble, not the link, and the
    message names include/labels.inc rather than anything hand-written:
    `symbol D_s18_80024074 is already defined'.
    """
    gone = []
    for f in sorted(glob.glob(os.path.join(PROJ, "csrc", "ovl_dng_*.c"))):
        unit = os.path.basename(f)[:-2]
        if only and unit not in only:
            continue
        text = open(f).read()
        for sym in re.findall(r"(?m)^const u8 (D_\w+)\[\]", text):
            if not splat_emits(unit, sym):
                continue
            pat = re.compile(
                r"(?:/\*[^*]*(?:\*(?!/)[^*]*)*\*/\n)?"
                r"const u8 %s\[\] = \{[^}]*\};\n+" % re.escape(sym))
            text, n = pat.subn("", text, count=1)
            if n:
                gone.append((unit, sym))
        open(f, "w").write(text)
    for u, sym in gone:
        print("pruned %-16s %s (splat emits it)" % (u, sym))
    print("%d duplicate const array(s) removed" % len(gone))


def blocked(text):
    """[(seg, first table offset, {sym: readers}, tables, lo, hi)]."""
    out = []
    for s in segments(text):
        if any(t == ".rodata" for _, t, _ in s["subs"]):
            continue
        tb = tables(s["name"])
        code = [int(a, 16) for a, t, _u in s["subs"] if t == "c"]
        if not tb or not code:
            continue
        code = code[0]
        first = min(tb)
        off = first - s["vram"] + s["start"]
        if off < s["start"] or off >= code:
            continue
        hi = code - s["start"] + s["vram"]
        refs = data_refs(s["name"], first, hi)
        if refs:
            out.append((s, off, refs, tb, first, hi))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--apply", action="store_true",
                    help="add the .rodata subsegments that need nothing else")
    ap.add_argument("--const", action="store_true",
                    help="write the const C arrays for the blocked units AND "
                         "add their .rodata subsegments")
    ap.add_argument("--prune", action="store_true",
                    help="remove const arrays that duplicate a symbol splat "
                         "emits; run after --const and a re-run of splat")
    ap.add_argument("--only", help="comma-separated unit names, for --const")
    a = ap.parse_args()
    text = open(YAML).read()

    if a.prune:
        prune(set(a.only.split(",")) if a.only else None)
        return

    if a.const:
        want = set(a.only.split(",")) if a.only else None
        done = 0
        for s, off, refs, tb, lo, hi in blocked(text):
            if want and s["name"] not in want:
                continue
            _READERS.clear()
            _READERS.update(refs)
            plan_ = blob_plan(s, lo, hi, tb, refs)
            # Skip any blob splat already defines for us (see splat_emits).
            plan_ = [p for p in plan_ if not splat_emits(s["name"], p[0])]
            if not plan_:
                print("%-16s nothing to write: splat emits every blob"
                      % s["name"])
            missing = [p[0] for p in plan_ if p[3] is None]
            if missing:
                print("SKIP %-16s no owning function for %s"
                      % (s["name"], ", ".join(missing)))
                continue
            n, fixed = write_consts(s, plan_)
            # and give the segment its .rodata subsegment
            lead = s["subs"][0]
            old = "      - [%s, %s]\n" % (lead[0], lead[1])
            line = "      - [%#x, .rodata, %s]\n" % (off, s["name"])
            blk = s["block"]
            newblk = blk.replace(old, old + line, 1) if int(lead[0], 16) != off \
                else blk.replace(old, line, 1)
            text = text.replace(blk, newblk, 1)
            print("%-16s %d const array(s)%s" % (s["name"], n,
                  (", declaration(s) made const: " + ", ".join(fixed))
                  if fixed else ""))
            done += 1
        open(YAML, "w").write(text)
        print("\n%d unit(s) given const rodata and a .rodata subsegment" % done)
        return

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
        print("(analysis only; pass --apply to edit %s, --const for the "
              "blocked units)" % os.path.relpath(YAML, PROJ))
        return
    text, n = apply(text, todo)
    open(YAML, "w").write(text)
    print("wrote %s: %d .rodata subsegment(s) added"
          % (os.path.relpath(YAML, PROJ), n))


if __name__ == "__main__":
    main()
