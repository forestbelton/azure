"""Locate PSY-Q library modules in the executable by content.

`libmatch` answers "which SDK release is this?" given subsegments that already
name their modules. This answers the inverse and more useful question: given
the right SDK release, *where* does each library module actually sit?

Module names and block boundaries guessed from a different SDK release are
wrong in two ways -- the name may not exist in the correct release at all, and
the boundary may be off. Searching by content fixes both at once, and finds
modules from libraries that were never identified.

Method. Every instruction the linker rewrites is known from the module's own
relocation table, so the remaining words are invariant between the `.LIB` copy
and the linked copy. Each module is anchored on its first run of four
consecutive non-relocated words; that 16-byte key is looked up in an index of
the executable, and every hit is then verified across the module's full length
with relocation masking. A module is reported only when the whole thing
matches, so a hit is a byte-level identification rather than a heuristic.

Tiny modules are intrinsically ambiguous (`SetPolyF3` is four instructions
that could sit anywhere), so the number of invariant words backing each
identification is reported, and multiple surviving candidates are flagged
rather than silently resolved.
"""

import argparse
import collections
import dataclasses
import pathlib
import struct
from typing import Optional

from azure import psylib, psyobj
from azure.commands.libmatch import LibraryIndex, parse_splat_yaml

ANCHOR_WORDS = 4
ANCHOR_BYTES = ANCHOR_WORDS * 4


@dataclasses.dataclass
class Located:
    lib: str
    module: str
    path: pathlib.Path
    rom_start: int
    length: int
    invariant_words: int

    @property
    def label(self) -> str:
        return f"{self.lib}/{self.module}"

    @property
    def rom_end(self) -> int:
        return self.rom_start + self.length

    @property
    def words(self) -> int:
        return self.length // 4


def word_masks(obj: psyobj.PsyQObject, nwords: int) -> list[int]:
    masks = [0] * nwords
    for reloc in obj.text_relocs():
        if not reloc.known or reloc.offset % 4 or reloc.offset // 4 >= nwords:
            continue
        masks[reloc.offset // 4] |= reloc.mask
    return masks


def find_anchor(masks: list[int]) -> Optional[int]:
    """First word index starting a run of ANCHOR_WORDS unrelocated words."""
    run = 0
    for i, mask in enumerate(masks):
        run = run + 1 if mask == 0 else 0
        if run == ANCHOR_WORDS:
            return i - ANCHOR_WORDS + 1
    return None


def build_index(exe: bytes, lo: int, hi: int) -> dict[bytes, list[int]]:
    index: dict[bytes, list[int]] = {}
    for off in range(lo, hi - ANCHOR_BYTES + 1, 4):
        index.setdefault(exe[off : off + ANCHOR_BYTES], []).append(off)
    return index


def verify(exe: bytes, base: int, text: bytes, masks: list[int]) -> bool:
    if base < 0 or base + len(text) > len(exe):
        return False
    for i, mask in enumerate(masks):
        keep = ~mask & 0xFFFFFFFF
        if keep == 0:
            continue
        a, = struct.unpack_from("<I", text, i * 4)
        b, = struct.unpack_from("<I", exe, base + i * 4)
        if (a & keep) != (b & keep):
            return False
    return True


def locate(
    exe: bytes,
    index: dict[bytes, list[int]],
    obj: psyobj.PsyQObject,
    lo: int,
    hi: int,
) -> tuple[list[int], int]:
    """(verified bases, invariant word count) for this module's .text."""
    text = obj.text()
    if text is None or len(text) < ANCHOR_BYTES:
        return [], 0
    nwords = len(text) // 4
    masks = word_masks(obj, nwords)
    invariant = sum(1 for m in masks if m == 0)

    anchor = find_anchor(masks)
    if anchor is None:
        return [], invariant

    key = text[anchor * 4 : anchor * 4 + ANCHOR_BYTES]
    bases = []
    for hit in index.get(key, ()):
        base = hit - anchor * 4
        if base < lo or base + len(text) > hi:
            continue
        if verify(exe, base, text, masks):
            bases.append(base)
    return bases, invariant


def scan(
    exe: bytes,
    release: pathlib.Path,
    lo: int,
    hi: int,
    only_libs: Optional[set[str]] = None,
) -> tuple[list[Located], list[Located], dict[str, int]]:
    """Returns (unique, ambiguous, stats)."""
    index = build_index(exe, lo, hi)
    unique: list[Located] = []
    ambiguous: list[Located] = []
    stats: collections.Counter = collections.Counter()

    lib_index = LibraryIndex(release)
    for lib in sorted(lib_index.names):
        if only_libs and lib not in only_libs:
            continue
        # One copy per library name: the shortest path wins.
        path = lib_index.candidates(lib)[0]
        try:
            with open(path, "rb") as f:
                library = psylib.PsyQLibrary.decode(f)
        except Exception:
            stats["unreadable libraries"] += 1
            continue
        for module in library.modules:
            stats["modules examined"] += 1
            try:
                obj = psyobj.PsyQObject.decode(module.data)
            except psyobj.PsyQObjException:
                stats["unparseable modules"] += 1
                continue
            if obj.truncated:
                stats["truncated parses"] += 1
            bases, invariant = locate(exe, index, obj, lo, hi)
            if not bases:
                continue
            text = obj.text()
            found = [
                Located(
                    lib=lib,
                    module=module.name,
                    path=path,
                    rom_start=base,
                    length=len(text),
                    invariant_words=invariant,
                )
                for base in bases
            ]
            if len(found) == 1:
                unique.append(found[0])
            else:
                ambiguous.extend(found)
    unique.sort(key=lambda m: m.rom_start)
    ambiguous.sort(key=lambda m: (m.label, m.rom_start))
    return unique, ambiguous, dict(stats)


def overlaps(located: list[Located]) -> list[tuple[Located, Located]]:
    out = []
    for a, b in zip(located, located[1:]):
        if b.rom_start < a.rom_end:
            out.append((a, b))
    return out


def setup_parser(prog: str) -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog=prog,
        description="""
            Locate PSY-Q library modules in the executable by content, using
            the correct SDK release. Emits splat subsegment lines with derived
            rather than guessed boundaries.
        """.strip(),
    )
    parser.add_argument(
        "release", type=pathlib.Path, help="the matching SDK release directory"
    )
    parser.add_argument(
        "--exe",
        type=pathlib.Path,
        default=pathlib.Path("data/slus_006.14"),
        help="the extracted executable to search",
    )
    parser.add_argument(
        "--yaml",
        type=pathlib.Path,
        default=pathlib.Path("splat/slus_006.14.yaml"),
        help="existing splat config, for a before/after comparison",
    )
    parser.add_argument(
        "--start", type=lambda v: int(v, 0), default=0x800,
        help="first file offset to search (default 0x800)",
    )
    parser.add_argument(
        "--end", type=lambda v: int(v, 0), default=0x55000,
        help="one past the last file offset to search (default 0x55000, the "
             "end of the loaded image per the PS-EXE header)",
    )
    parser.add_argument(
        "--lib", action="append", dest="libs",
        help="restrict to this library (repeatable)",
    )
    parser.add_argument(
        "--min-invariant", type=int, default=4,
        help="suppress identifications backed by fewer invariant words",
    )
    parser.add_argument(
        "--emit", action="store_true",
        help="print splat subsegment lines instead of the report",
    )
    parser.set_defaults(main=main)
    return parser


def main(args: argparse.Namespace) -> None:
    with open(args.exe, "rb") as f:
        exe = f.read()
    only = {l.lower() for l in args.libs} if args.libs else None
    unique, ambiguous, stats = scan(exe, args.release, args.start, args.end, only)

    weak = [m for m in unique if m.invariant_words < args.min_invariant]
    strong = [m for m in unique if m.invariant_words >= args.min_invariant]

    if args.emit:
        for m in strong:
            print(f"      - [{m.rom_start:#07x}, asm, psyq/{m.lib}/{m.module}]")
        return

    existing = {r.label: r for r in parse_splat_yaml(args.yaml)}
    by_label = {m.label: m for m in strong}

    print(f"searched {args.start:#x}-{args.end:#x} of {args.exe}")
    print(f"release: {args.release}")
    for k, v in sorted(stats.items()):
        print(f"  {k}: {v}")
    print(
        f"\nlocated {len(strong)} modules "
        f"({len(weak)} suppressed below {args.min_invariant} invariant words, "
        f"{len(ambiguous)} ambiguous)"
    )

    confirmed = [m for m in strong if m.label in existing
                 and existing[m.label].rom_start == m.rom_start]
    moved = [m for m in strong if m.label in existing
             and existing[m.label].rom_start != m.rom_start]
    new = [m for m in strong if m.label not in existing]
    gone = [lbl for lbl in existing if lbl not in by_label]

    print(f"  confirmed at the current offset: {len(confirmed)}")
    print(f"  found at a different offset:     {len(moved)}")
    print(f"  not currently in the config:     {len(new)}")
    print(f"  in the config but not located:   {len(gone)}")

    if moved:
        print("\n=== boundaries to correct ===")
        for m in moved:
            delta = m.rom_start - existing[m.label].rom_start
            print(f"  {m.label:<24} {existing[m.label].rom_start:#08x} -> "
                  f"{m.rom_start:#08x} ({delta:+#x})  "
                  f"{m.words}w, {m.invariant_words} invariant")

    if new:
        print(f"\n=== newly identified ({len(new)}) ===")
        for lib, group in sorted(
            collections.ChainMap(
                *[{m.lib: None} for m in new]
            ).items()
        ):
            members = [m for m in new if m.lib == lib]
            print(f"  {lib} ({len(members)} modules)")
            for m in members[:12]:
                print(f"    {m.rom_start:#08x}  {m.module:<14} "
                      f"{m.words:>5}w  {m.invariant_words} invariant")
            if len(members) > 12:
                print(f"    ... and {len(members) - 12} more")

    clashes = overlaps(strong)
    if clashes:
        print(f"\n=== {len(clashes)} overlapping identifications ===")
        for a, b in clashes[:20]:
            print(f"  {a.label} [{a.rom_start:#x}-{a.rom_end:#x}) overlaps "
                  f"{b.label} [{b.rom_start:#x}-{b.rom_end:#x})")

    if ambiguous:
        seen: collections.Counter = collections.Counter(m.label for m in ambiguous)
        print(f"\n=== ambiguous ({len(seen)} modules matched in >1 place) ===")
        for label, n in seen.most_common(12):
            places = [m.rom_start for m in ambiguous if m.label == label]
            inv = next(m.invariant_words for m in ambiguous if m.label == label)
            print(f"  {label:<24} {n} sites, {inv} invariant words: "
                  f"{', '.join(hex(p) for p in places[:6])}")

    if strong:
        covered = sum(m.length for m in strong)
        print(f"\ncoverage: {covered:#x} bytes of {args.end - args.start:#x} "
              f"({100 * covered / (args.end - args.start):.1f}%)")


if __name__ == "__main__":
    parser = setup_parser("libscan")
    main(parser.parse_args())
