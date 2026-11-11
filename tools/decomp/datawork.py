#!/usr/bin/env python3
"""datawork.py -- the town overlay's data queue, and the brief for one park.

`worklist.py`'s job for DATA. It reads the current state off the yaml and the
listing every time, so nothing in its output can go stale.

    python3 tools/decomp/datawork.py                     # where the queue is
    python3 tools/decomp/datawork.py --count 30          # the next park's objects
    python3 tools/decomp/datawork.py --count 30 --brief dq2_   # the agent brief

THE QUEUE IS STRICTLY ORDERED, which is the one way this differs from the
function sweep: there is nothing to rank. A C unit's `.data` is the region's
contiguous PREFIX (docs/owning-data.md §8), so the only choice is how many
objects the next park takes, and the answer is "as many as fit in one
reading". The objects at the front are not the easiest ones, they are the only
ones.

`--count` therefore sets the park's size, and the natural places to stop are
printed with the table: a park should end where a run of same-shaped objects
ends, not in the middle of one.
"""
import argparse
import os
import re

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.path.dirname(os.path.dirname(HERE))

LISTING = os.path.join(PROJ, "asm/data/ovl_town_data.data.s")
YAML = os.path.join(PROJ, "splat/town.bin.yaml")
UNITS = ["csrc/ovl_town.c", "csrc/ovl_town_800BEBA4.c", "csrc/ovl_town_800BF3F0.c"]
ROM0, VRAM0 = 0x51FC0, 0x800CF720


def window():
    """(rom start, rom end) of the `.data` subsegment the yaml declares now."""
    txt = open(YAML).read()
    m = re.search(r"- \[(0x[0-9a-fA-F]+), \.data, ovl_town\]", txt)
    if not m:
        raise SystemExit("splat/town.bin.yaml has no `.data, ovl_town` subsegment")
    start = int(m.group(1), 16)
    m2 = re.search(r"- \[(0x[0-9a-fA-F]+), data, ovl_town_data\]", txt)
    if not m2:
        raise SystemExit("splat/town.bin.yaml has no `data, ovl_town_data` subsegment")
    return start, int(m2.group(1), 16)


def declarations():
    """vram -> {the `extern` spellings the three units use}."""
    out = {}
    for u in UNITS:
        s = re.sub(r"/\*.*?\*/", " ", open(os.path.join(PROJ, u)).read(), flags=re.S)
        for m in re.finditer(r"^extern\s+([^;]*?\bD_([0-9A-F]{8})\b[^;]*);", s, re.M):
            out.setdefault(int(m.group(2), 16), set()).add(
                re.sub(r"\s+", " ", m.group(1).strip()))
    return out


def owned():
    """The D_ objects csrc/ovl_town.c already defines, in address order.

    NOT from the listing: splat stops emitting an object once the `.data`
    window covers it, so the listing only ever shows what is LEFT. The last
    of these is the merge anchor for the next park.
    """
    s = open(os.path.join(PROJ, "csrc/ovl_town.c")).read()
    found = re.findall(r"^[A-Za-z_][\w \*]*?\bD_([0-9A-F]{8})\s*(?:\[[^\]]*\])?\s*=",
                       s, re.M)
    # The unit also owns rodata objects (migrated string literals), which are
    # definitions of the same shape at a lower address. Only the data region
    # counts.
    return sorted(a for a in {int(x, 16) for x in found}
                  if VRAM0 <= a < VRAM0 + (0x7E000 - ROM0))


def objects():
    """[(vram, size, width, nwords, nonzero, [symbols])] from the listing."""
    lines = open(LISTING).read().splitlines()
    labs = [(int(m.group(1), 16), i)
            for i, l in enumerate(lines)
            for m in [re.match(r"dlabel D_([0-9A-F]{8})$", l)] if m]
    out = []
    for k, (a, i) in enumerate(labs):
        j, width, n, nz, syms = i + 1, set(), 0, 0, []
        while j < len(lines) and not lines[j].startswith(
                ("dlabel", "enddlabel", "nonmatching")):
            t = lines[j].strip()
            m = re.match(r"/\*.*?\*/\s*\.(word|short|byte)\s+(.*)$", t)
            if m:
                width.add(m.group(1))
                n += 1
                v = m.group(2).strip()
                if re.fullmatch(r"0x0+", v):
                    pass
                else:
                    nz += 1
                if not re.fullmatch(r"0x[0-9A-Fa-f]+", v):
                    syms.append(v)
            j += 1
        nxt = labs[k + 1][0] if k + 1 < len(labs) else None
        size = (nxt - a) if nxt else n * {"word": 4, "short": 2, "byte": 1}[
            next(iter(width))]
        out.append((a, size, "/".join(sorted(width)), n, nz, syms))
    return out


HARD = """## HARD RULES -- violating any destroys shared state
- Do **NOT** touch `csrc/`, `include/`, `splat/` or `docs/`. You produce a merge
  script and one yaml line; I own the merge, the build gate and the commit.
- Do **NOT** run `make`, `make check` or `make splat`. **NEVER** `rm -rf build`.
  **NEVER** `splat split --modes ld`.
- Do **NOT** commit.
- Everything to `scratch/` with the UNIQUE prefix `%(pre)s`.
- The overlay compiles **`-G0`, not `-G8`**. `dataverify.py` already passes it;
  any helper you write must too.
- `csrc/ovl_town.c` may move under you. Anchor-based, re-runnable merge script;
  assert every anchor BEFORE writing anything; re-run it at the end.
"""

RULES = """## THE RULES FOR THIS WORK -- all of them measured, see docs/owning-data.md §8
1. **ONE OBJECT PER `dlabel`. NEVER MERGE TWO.** A run of same-sized records
   is not one array: a pointer table elsewhere in the region points at each
   one separately, and 164 functions in this overlay are still asm stubs that
   reference these symbols by name. Merging drops a symbol the link needs.
2. **The type is the one the unit already declared.** The table below prints
   it. Writing a different one is a `conflicting types` error against the
   existing `extern` -- that is the gate working, not an obstacle to route
   around. Where the column is empty the sweeps never needed a declaration,
   and then the listing's element width is all you may claim: `s32[21]` for an
   undeclared 84-byte word record, `u8[n]` for a byte one.
3. **Write the length explicitly.** `[16]`, never `[]`.
4. **An explicitly zero initialiser lands in `.data` under this compiler**, so
   a zero-filled object is owned like any other and an all-zero struct needs
   no field layout -- `= { 0 }` is enough. (The `.bss` placement of zero
   initialisers is a later-gcc optimisation; cc1psx 2.7.2 does not do it.)
   Leaving an object with NO initialiser puts it in `.bss` and
   `dataverify.py` will tell you it found no `.data` symbols.
5. **Keep every existing `extern`.** The definitions go at the FOOT of the
   file, so every use site is above them and must keep seeing the declaration
   it saw before.
6. **A symbol operand stays a symbol.** Where the listing says
   `.word func_8008BA44` or `.word D_800CF880`, the C says the same name --
   with the cast the declared type needs, e.g. `(s32)func_8008BA44`. A raw
   `0x80080EE0` in the listing stays a literal; do not invent a symbol for it.
   A scalar `extern` needs its address taken: `&D_80080EA8`, not a cast.
7. **A gap between two objects is PADDING and it is yours to carry.** The
   object's span in the table is the distance to the NEXT `dlabel`, which can
   be larger than the declared object: `u8 D_800CFCF3` is one byte with a
   12-byte gap after it. cc1psx will not insert that gap -- a `u8` array needs
   no alignment -- so the next object lands 12 bytes early and
   `dataverify.py` reports the drift. Size the object to its span
   (`u8 D_800CFCF3[13]`) and say in your hand-back that the length is the
   SPAN and not a measured size.
8. **A standalone defs file needs its own declarations.** Anything an
   initialiser names -- `extern void func_8008BA44();`, `extern u8 D_80088760[];`
   -- has to be declared in your scratch file or the compile fails on
   "initializer element is not constant". Those preamble lines are for
   `dataverify.py` only and do NOT go in the merge script unless the unit is
   actually missing the declaration.
"""

VERIFY = """## HOW TO VERIFY -- this is a real gate, use it on every object
1. Write the definitions to `scratch/%(pre)sdefs.c` with the preamble rule 8
   describes, then:

       python3 tools/decomp/dataverify.py scratch/%(pre)sdefs.c

   It compiles at `-G0`, takes the object's `.data`, and compares it against
   `data/town/town.bin` with relocated words masked. Three things it prints
   and what each means:

   * **`layout`** must read `ok` for every object. A `+0x4` means an object
     ABOVE it has the wrong length -- the drift starts at the first object
     after the mistake, so read the first non-`ok` row and look at the row
     before it.
   * **`bytes`** must read `OK`. `N DIFFER at +0x..` is a wrong value.
   * **`NEXT YAML BOUNDARY`** is the number to hand back. Nothing else.

2. **Read every relocation it lists against the symbol's own listing.** The
   masked words are exactly the ones the tool cannot check, which is the same
   bargain `ovlverify.py` makes for text. A relocation against the wrong
   symbol that happens to resolve to the right word is invisible here.

3. Re-run it once more at the end, on the FULL set, after any edit.
"""

BACK = """## WHAT TO HAND BACK
1. The merge script path, and `dataverify.py`'s final output pasted whole.
2. **The one yaml number**: the `NEXT YAML BOUNDARY`, for the
   `- [..., data, ovl_town_data]` line. Do not edit the yaml yourself.
3. **Per object: the type you wrote and where it came from** -- the existing
   `extern`, or the listing's element width. Say which.
4. **Every relocation, with the listing you checked it against.**
5. Anything you did not do, with the reason. An object whose declared type you
   could not satisfy is a fine outcome to report: stop the park BELOW it and
   hand back the shorter boundary. A short park that verifies beats a long one
   that does not, and the queue is a prefix so stopping early costs nothing.
6. **Errors in this brief or in either tool.** `dataverify.py` is new; it has
   one self-test (the landed tranche) and one negative test behind it and
   nothing else.
"""


def main() -> None:
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--count", type=int, default=0,
                    help="objects in the next park (0: just print the queue)")
    ap.add_argument("--brief", metavar="PREFIX",
                    help="emit an agent brief using this scratch prefix")
    args = ap.parse_args()

    win_start, win_end = window()
    owned_vram = VRAM0 + (win_end - win_start)
    objs = objects()
    done = owned()
    left = [o for o in objs if o[0] >= owned_vram]
    region_end = VRAM0 + (0x7E000 - ROM0)
    decl = declarations()

    print(f"ovl_town data: owned {win_start:#x}..{win_end:#x}  "
          f"({win_end - win_start} of {0x7E000 - ROM0} bytes, "
          f"{(win_end - win_start) / (0x7E000 - ROM0):.1%})")
    print(f"  objects: {len(done)} owned, {len(left)} to go")
    print(f"  next object D_{left[0][0]:08X} at rom {win_end:#x}" if left
          else "  the region is fully owned")
    if not left:
        return

    n = args.count or 12
    park = left[:n]
    nxt_vram = left[n][0] if len(left) > n else region_end
    print(f"\nnext park: {len(park)} objects, "
          f"D_{park[0][0]:08X}..D_{park[-1][0]:08X}, "
          f"{nxt_vram - park[0][0]} bytes; "
          f"boundary would be {ROM0 + (nxt_vram - VRAM0):#x}")

    rows = []
    for a, size, width, nw, nz, syms in park:
        d = decl.get(a)
        ty = " | ".join(sorted(d)) if d else ""
        flag = []
        if d and len(d) > 1:
            flag.append("CONFLICTING DECLARATIONS")
        if nz == 0:
            flag.append("all zero")
        if syms:
            flag.append(f"{len(syms)} symbol operand{'s' * (len(syms) != 1)}")
        rows.append((f"D_{a:08X}", size, width, nw, ty, "; ".join(flag)))

    w = max(len(r[4]) for r in rows) if rows else 0
    print(f"\n  {'object':12} {'bytes':>6} {'elem':>10} {'n':>4}  "
          f"{'declared type':{w}}  notes")
    for nm, size, width, nw, ty, flag in rows:
        print(f"  {nm:12} {size:>6} {width:>10} {nw:>4}  {ty:{w}}  {flag}")

    # Where a run of same-shaped objects ends is a better park boundary than a
    # count, so say where the next few are.
    stops = []
    for i in range(1, min(len(left), 80)):
        if (left[i][1], left[i][2]) != (left[i - 1][1], left[i - 1][2]):
            stops.append(i)
    print("\n  shape changes after object #" + ", #".join(str(s) for s in stops[:8])
          + "  (good --count values)")

    if args.brief:
        pre = args.brief
        print("\n" + "=" * 72)
        print(f"# Park: own ovl_town's data objects "
              f"D_{park[0][0]:08X}..D_{park[-1][0]:08X}\n")
        print(f"The town overlay's data is being moved out of "
              f"`asm/data/ovl_town_data.data.s` and into `csrc/ovl_town.c`, "
              f"which emits the first `.data` of the segment. "
              f"{win_end - win_start} of {0x7E000 - ROM0} bytes are owned; "
              f"this park takes the next {len(park)} objects, "
              f"`0x{win_end:X}..0x{ROM0 + (nxt_vram - VRAM0):X}`.\n")
        print("Read `docs/owning-data.md` §8 first -- it is short and it is the "
              "whole method. `docs/town-map.md` is the container map if you want "
              "to know what the region is.\n")
        print(HARD % {"pre": pre})
        print(RULES)
        print("## THE OBJECTS\n")
        print(f"Append in this order at the FOOT of `csrc/ovl_town.c`, after the "
              f"last object of the previous tranche (`D_{done[-1]:08X}`), "
              f"which is your merge anchor.\n")
        print(f"```\n  {'object':12} {'bytes':>6} {'elem':>10} {'n':>4}  "
              f"{'declared type':{w}}  notes")
        for nm, size, width, nw, ty, flag in rows:
            print(f"  {nm:12} {size:>6} {width:>10} {nw:>4}  {ty:{w}}  {flag}")
        print("```\n")
        after = (f"/^dlabel D_{left[len(park)][0]:08X}$/"
                 if len(left) > len(park) else "0")
        print(f"The bytes are in the listing. Read the whole park with\n\n"
              f"    awk '/^dlabel D_{park[0][0]:08X}$/,{after}' "
              f"asm/data/ovl_town_data.data.s\n")
        print(VERIFY % {"pre": pre})
        print(BACK)


if __name__ == "__main__":
    main()
