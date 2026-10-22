"""Identify which PSY-Q SDK release a game was linked against.

Library modules ship pre-built in the SDK, so their code is independent of the
compiler version and flags used for the game's own translation units. That
makes them an unambiguous fingerprint -- but only if the comparison accounts
for relocation. The `.LIB` copy of a module holds placeholders in every
instruction that references an external symbol or an absolute address; the
copy linked into the executable holds resolved values. A plain byte compare
therefore mismatches at exactly those words, which looks like a near-miss on
the SDK version when in fact it is a perfect match.

Two comparisons are reported per module:

  masked   Relocation-aware exact match. The object's relocation table says
           which bits the linker rewrites; those are masked out of both sides
           and everything else must be identical. This is the authoritative
           result.

  fuzzy    Fraction of instruction words that are equal, or that differ only
           in a field a relocation could plausibly have rewritten (the low 16
           bits of an immediate, the 26-bit jump target). Needs no relocation
           table, so it still discriminates when a module's object had to be
           parsed tolerantly. Branches are compared strictly, since
           intra-module branch displacements are PC-relative and are not
           relocated.
"""

import argparse
import collections
import dataclasses
import pathlib
import re
import struct
from typing import Optional

import yaml

from azure import psylib, psyobj

# Opcodes whose low 16 bits can carry a relocated value. Branch opcodes
# (0x01, 0x04-0x07) are deliberately excluded: their displacements are
# PC-relative and must match exactly.
IMM16_OPS = frozenset(
    [0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F]  # arithmetic / logical / lui
    + [0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26]  # loads
    + [0x28, 0x29, 0x2A, 0x2B, 0x2E]  # stores
    + [0x30, 0x31, 0x32, 0x33]  # ll, lwc1-3 (lwc2 = GTE)
    + [0x38, 0x39, 0x3A, 0x3B]  # sc, swc1-3 (swc2 = GTE)
)
JUMP_OPS = frozenset([0x02, 0x03])  # j, jal


def word_equivalent(a: int, b: int) -> bool:
    """True if `a` and `b` could be the same instruction pre/post relocation."""
    if a == b:
        return True
    opcode = a >> 26
    if opcode != b >> 26:
        return False
    if opcode in JUMP_OPS:
        return True
    if opcode in IMM16_OPS:
        # Top 16 bits are opcode/rs/rt; only the immediate may differ.
        return (a >> 16) == (b >> 16)
    return False


@dataclasses.dataclass
class Comparison:
    words: int
    masked_exact: bool
    fuzzy: float
    first_bad: Optional[int]


def compare_text(
    module_text: bytes, exe_text: bytes, relocs: list[psyobj.Reloc]
) -> Comparison:
    length = min(len(module_text), len(exe_text)) & ~3
    word_count = length // 4
    if word_count == 0:
        return Comparison(words=0, masked_exact=False, fuzzy=0.0, first_bad=None)

    masks = [0] * word_count
    for reloc in relocs:
        if reloc.offset % 4 != 0 or reloc.offset + 4 > length:
            continue
        masks[reloc.offset // 4] |= reloc.mask

    masked_exact = True
    equivalent = 0
    first_bad: Optional[int] = None
    for i in range(word_count):
        a, = struct.unpack_from("<I", module_text, i * 4)
        b, = struct.unpack_from("<I", exe_text, i * 4)
        keep = ~masks[i] & 0xFFFFFFFF
        if (a & keep) == (b & keep):
            equivalent += 1
            continue
        masked_exact = False
        if word_equivalent(a, b):
            equivalent += 1
        elif first_bad is None:
            first_bad = i * 4

    return Comparison(
        words=word_count,
        masked_exact=masked_exact,
        fuzzy=equivalent / word_count,
        first_bad=first_bad,
    )


@dataclasses.dataclass
class ModuleRef:
    """A PSY-Q library module that `splat` has located in the executable."""

    lib: str
    module: str
    rom_start: int
    rom_end: int

    @property
    def label(self) -> str:
        return f"{self.lib}/{self.module}"


def parse_splat_yaml(path: pathlib.Path) -> list[ModuleRef]:
    with open(path) as f:
        doc = yaml.safe_load(f)

    segments = doc.get("segments") or []
    refs: list[ModuleRef] = []
    for seg_index, segment in enumerate(segments):
        if not isinstance(segment, dict):
            continue
        subsegments = segment.get("subsegments")
        if not subsegments:
            continue

        # Bound for the final subsegment: the start of the next top-level
        # segment, which splat spells as a bare [addr] terminator.
        tail = None
        for later in segments[seg_index + 1 :]:
            if isinstance(later, list) and later:
                tail = later[0]
                break

        starts: list[tuple[int, Optional[str], Optional[str]]] = []
        for entry in subsegments:
            if not isinstance(entry, list) or not entry:
                continue
            start = entry[0]
            kind = entry[1] if len(entry) > 1 else None
            name = entry[2] if len(entry) > 2 else None
            starts.append((start, kind, name))

        for i, (start, kind, name) in enumerate(starts):
            if kind != "asm" or not name or not name.startswith("psyq/"):
                continue
            parts = name.split("/")
            if len(parts) != 3:
                continue
            end = starts[i + 1][0] if i + 1 < len(starts) else tail
            if end is None:
                continue
            refs.append(
                ModuleRef(lib=parts[1], module=parts[2], rom_start=start, rom_end=end)
            )
    return refs


def parse_symbol_addrs(path: pathlib.Path) -> dict[str, int]:
    """name -> vram, from a splat symbol_addrs file."""
    symbols: dict[str, int] = {}
    if not path.exists():
        return symbols
    for line in path.read_text().splitlines():
        line = line.split("//")[0].strip()
        if "=" not in line:
            continue
        name, _, value = line.partition("=")
        try:
            symbols[name.strip()] = int(value.strip().rstrip(";"), 16)
        except ValueError:
            continue
    return symbols


@dataclasses.dataclass
class Alignment:
    """Where a module's .text was found to sit, and how well corroborated."""

    rom_base: int
    votes: int
    total: int
    source: str          # "exports" or "splat"
    span_delta: int = 0  # rom_base - splat subsegment start

    @property
    def confident(self) -> bool:
        """Several independent exports agree, so this base is not a guess."""
        return self.source == "exports" and self.votes >= 3


def candidate_bases(
    obj: "psyobj.PsyQObject",
    ref: ModuleRef,
    symbols: dict[str, int],
    vram_base: int,
    rom_header: int,
) -> list[tuple[int, int, str]]:
    """Plausible .text bases for this module, as (rom_base, votes, source).

    Each export with a known address votes for `addr - export.offset`. When the
    module is the same build as the one in the executable every export agrees;
    when it is a *different* build the function sizes differ and the votes
    scatter, so no single base aligns everything. Rather than guess, every
    candidate is returned and the caller keeps whichever scores best -- so a
    low score means the code genuinely differs, not that we mis-aligned.
    """
    votes: collections.Counter = collections.Counter()
    if obj.sections.get(".text") is not None:
        for export in obj.text_exports():
            if export.name not in symbols:
                continue
            votes[symbols[export.name] - export.offset - vram_base + rom_header] += 1

    out = [(base, n, "exports") for base, n in votes.most_common()]
    if ref.rom_start not in votes:
        out.append((ref.rom_start, 0, "splat"))
    return out


class LibraryIndex:
    """Case-insensitive index of every .LIB beneath a release directory.

    Releases disagree on case and layout: 4.3 ships `lib/libgte.lib`, 4.4 ships
    `psx/lib/LIBGTE.LIB` plus a second copy under `psx/lib/patches/`, and 4.7
    ships `LIB/Libcd.lib`. Every candidate for a name is kept so the caller can
    try them all.
    """

    def __init__(self, root: pathlib.Path) -> None:
        self.root = root
        self.names: dict[str, list[pathlib.Path]] = {}
        for path in sorted(root.rglob("*")):
            if not path.is_file() or path.suffix.lower() != ".lib":
                continue
            self.names.setdefault(path.stem.lower(), []).append(path)

    def candidates(self, lib: str) -> list[pathlib.Path]:
        """Shortest path first, so `psx/lib/X` beats `psx/sample/.../X`."""
        return sorted(
            self.names.get(lib.lower(), []), key=lambda p: (len(p.parts), str(p))
        )


RELEASE_DIR = re.compile(r"^v?\d+(\.\d+)*$")


def find_releases(root: pathlib.Path) -> list[pathlib.Path]:
    """The SDK releases under `root`, or `root` itself if it is a single one.

    Only version-looking subdirectory names ("4.0", "4.7", "3.6") count as
    releases. Without that test a single unpacked release is mistaken for
    several, because its own `psx/`, `psyq/` and `compiler/` directories each
    contain libraries.
    """
    subdirs = [
        p
        for p in sorted(root.iterdir())
        if p.is_dir() and RELEASE_DIR.match(p.name)
    ]
    releases = [p for p in subdirs if LibraryIndex(p).names]
    return releases or [root]


class ModuleLoader:
    """Extracts and parses library modules, with caching."""

    def __init__(self) -> None:
        self._libs: dict[str, Optional[psylib.PsyQLibrary]] = {}
        self._indexes: dict[str, LibraryIndex] = {}
        self.missing_libs: set[tuple[str, str]] = set()
        self.truncated: set[tuple[str, str]] = set()

    def index(self, release: pathlib.Path) -> LibraryIndex:
        key = str(release)
        if key not in self._indexes:
            self._indexes[key] = LibraryIndex(release)
        return self._indexes[key]

    def _library(self, path: pathlib.Path) -> Optional[psylib.PsyQLibrary]:
        key = str(path)
        if key not in self._libs:
            try:
                with open(path, "rb") as f:
                    self._libs[key] = psylib.PsyQLibrary.decode(f)
            except Exception:
                self._libs[key] = None
        return self._libs[key]

    def load(
        self, release: pathlib.Path, lib: str, module: str
    ) -> list[tuple[pathlib.Path, psyobj.PsyQObject]]:
        """Every parse of `module` found in any candidate copy of `lib`."""
        candidates = self.index(release).candidates(lib)
        if not candidates:
            self.missing_libs.add((str(release), lib))
            return []
        found: list[tuple[pathlib.Path, psyobj.PsyQObject]] = []
        for path in candidates:
            library = self._library(path)
            if library is None:
                continue
            for candidate in library.modules:
                if candidate.name.upper() != module.upper():
                    continue
                try:
                    obj = psyobj.PsyQObject.decode(candidate.data)
                except psyobj.PsyQObjException:
                    break
                if obj.truncated:
                    self.truncated.add((lib, module))
                found.append((path, obj))
                break
        return found


@dataclasses.dataclass
class Result:
    ref: ModuleRef
    comparison: Optional[Comparison]
    note: str = ""
    source: Optional[pathlib.Path] = None
    alignment: Optional[Alignment] = None


def _rank(result: Result) -> tuple:
    return (
        result.comparison is not None,
        result.comparison.masked_exact if result.comparison else False,
        result.comparison.fuzzy if result.comparison else -1.0,
    )


def evaluate_release(
    release: pathlib.Path,
    refs: list[ModuleRef],
    exe: bytes,
    loader: ModuleLoader,
    symbols: Optional[dict[str, int]] = None,
    vram_base: int = 0x8002D000,
    rom_header: int = 0x800,
) -> list[Result]:
    symbols = symbols or {}
    results: list[Result] = []
    for ref in refs:
        objs = loader.load(release, ref.lib, ref.module)
        if not objs:
            results.append(Result(ref=ref, comparison=None, note="module not found"))
            continue

        best: Optional[Result] = None
        for path, obj in objs:
            text = obj.text()
            if text is None or len(text) < 4:
                candidate = Result(ref=ref, comparison=None, note="no .text")
                candidate.source = path
                if best is None or _rank(candidate) > _rank(best):
                    best = candidate
                continue

            relocs = [r for r in obj.text_relocs() if r.known]
            bases = candidate_bases(obj, ref, symbols, vram_base, rom_header)
            total_votes = sum(n for _, n, src in bases if src == "exports")

            candidate = None
            for base, nvotes, src in bases:
                if base < 0 or base >= len(exe):
                    continue
                window = exe[base : base + len(text)]
                comparison = compare_text(text, window, relocs)
                alignment = Alignment(
                    rom_base=base,
                    votes=nvotes,
                    total=total_votes,
                    source=src,
                    span_delta=base - ref.rom_start,
                )
                notes = []
                if obj.truncated:
                    notes.append("INCOMPLETE object parse")
                if not relocs:
                    notes.append("no relocs")
                if alignment.span_delta:
                    notes.append(f"base {base:#x} (span {alignment.span_delta:+#x})")
                trial = Result(
                    ref=ref, comparison=comparison, note=", ".join(notes)
                )
                trial.source = path
                trial.alignment = alignment
                if candidate is None or _rank(trial) > _rank(candidate):
                    candidate = trial
            if candidate is None:
                candidate = Result(ref=ref, comparison=None, note="no usable base")
                candidate.source = path
            if best is None or _rank(candidate) > _rank(best):
                best = candidate
        results.append(best)
    return results


def report(
    release: pathlib.Path, results: list[Result], verbose: bool
) -> tuple[int, int, float]:
    scored = [r for r in results if r.comparison is not None and r.comparison.words]
    exact = sum(1 for r in scored if r.comparison.masked_exact)
    mean_fuzzy = (
        sum(r.comparison.fuzzy for r in scored) / len(scored) if scored else 0.0
    )

    print(
        f"{release.name:<12} {exact:>5}/{len(scored):<5} {mean_fuzzy:>7.3f}   "
        f"{len(results) - len(scored):>8}"
    )
    if not verbose:
        return exact, len(scored), mean_fuzzy

    for result in sorted(results, key=_rank):
        ref = result.ref
        if result.comparison is None:
            print(f"    {ref.label:<28} --      {result.note}")
            continue
        comparison = result.comparison
        flag = "MATCH" if comparison.masked_exact else "     "
        detail = f"{comparison.fuzzy:6.3f} {comparison.words:>5}w {flag}"
        if comparison.first_bad is not None:
            addr = ref.rom_start + comparison.first_bad
            detail += f" bad +{comparison.first_bad:#06x} (file {addr:#08x})"
        if result.note:
            detail += f" [{result.note}]"
        print(f"    {ref.label:<28} {detail}")
    return exact, len(scored), mean_fuzzy


def setup_parser(prog: str) -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog=prog,
        description="""
            Determine which PSY-Q SDK release the game was linked against, by
            comparing pre-built library modules against the code splat has
            already located in the executable. The comparison is
            relocation-aware, so modules that call out to other objects or
            touch globals still match exactly.
        """.strip(),
    )
    parser.add_argument(
        "psyq_root",
        type=pathlib.Path,
        help="directory holding one subdirectory per SDK release",
    )
    parser.add_argument(
        "--yaml",
        type=pathlib.Path,
        default=pathlib.Path("splat/slus_006.14.yaml"),
        help="splat config naming the psyq/<lib>/<module> subsegments",
    )
    parser.add_argument(
        "--exe",
        type=pathlib.Path,
        default=pathlib.Path("data/slus_006.14"),
        help="the extracted executable the splat config describes",
    )
    parser.add_argument(
        "--symbols",
        type=pathlib.Path,
        default=pathlib.Path("splat/slus_006.14_symbol_addrs.txt"),
        help="splat symbol_addrs file, used to align modules by their exports",
    )
    parser.add_argument(
        "--vram",
        type=lambda v: int(v, 0),
        default=0x8002D000,
        help="vram base of the code segment (default 0x8002D000)",
    )
    parser.add_argument(
        "--module",
        action="append",
        dest="modules",
        help="only consider this module (repeatable), e.g. MTX_003",
    )
    parser.add_argument(
        "--lib",
        action="append",
        dest="libs",
        help="only consider this library (repeatable), e.g. libgte",
    )
    parser.add_argument("-v", "--verbose", action="store_true")
    parser.set_defaults(main=main)
    return parser


def main(args: argparse.Namespace) -> None:
    refs = parse_splat_yaml(args.yaml)
    if args.modules:
        wanted = {m.upper() for m in args.modules}
        refs = [r for r in refs if r.module.upper() in wanted]
    if args.libs:
        wanted_libs = {l.lower() for l in args.libs}
        refs = [r for r in refs if r.lib.lower() in wanted_libs]
    if not refs:
        print(f"no psyq/<lib>/<module> subsegments found in {args.yaml}")
        return

    with open(args.exe, "rb") as f:
        exe = f.read()

    releases = find_releases(args.psyq_root)
    if not releases:
        print(f"no SDK releases found under {args.psyq_root}")
        return

    loader = ModuleLoader()
    symbols = parse_symbol_addrs(args.symbols)
    libs = sorted({r.lib for r in refs})
    print(f"{len(refs)} located modules from {args.yaml}")
    print(f"{len(symbols)} symbols from {args.symbols}")
    print(f"libraries: {', '.join(libs)}\n")
    print(f"{'release':<12} {'exact':>11} {'fuzzy':>7} {'unscored':>11}")
    print("-" * 46)

    misaligned: dict[str, Alignment] = {}
    ranking: list[tuple[int, int, float, pathlib.Path]] = []
    for release in releases:
        results = evaluate_release(
            release, refs, exe, loader, symbols, args.vram
        )
        for r in results:
            if r.alignment and r.alignment.confident and r.alignment.span_delta:
                misaligned[r.ref.label] = r.alignment
        exact, scored, mean_fuzzy = report(release, results, args.verbose)
        ranking.append((exact, scored, mean_fuzzy, release))

    # Rank by the *rate* of exact matches, not the raw count: a release that
    # supplies fewer of the modules we need can still be the right one, and a
    # release that supplies many but matches half of them plainly is not.
    # Releases offering too few scored modules to be meaningful rank last.
    MIN_SCORED = 20
    ranking.sort(
        key=lambda row: (
            row[1] >= MIN_SCORED,
            row[0] / row[1] if row[1] else 0.0,
            row[2],
        ),
        reverse=True,
    )
    if ranking and ranking[0][0] > 0:
        exact, scored, mean_fuzzy, best = ranking[0]
        print(
            f"\nbest: {best.name} -- {exact}/{scored} exact "
            f"({100 * exact / scored:.1f}%), mean fuzzy {mean_fuzzy:.3f}"
        )
    else:
        print("\nno release produced an exact match; re-run with --verbose")

    if misaligned:
        print(f"\n{len(misaligned)} module(s) whose exports disagree with the "
              f"splat subsegment start:")
        for label, a in sorted(misaligned.items()):
            print(f"  {label:<24} exports say {a.rom_base:#08x} "
                  f"({a.span_delta:+#x}, {a.votes}/{a.total} votes)")
    if loader.truncated:
        print(
            f"note: {len(loader.truncated)} module(s) needed a tolerant "
            f"(truncated) object parse"
        )
    if loader.missing_libs:
        names = sorted({lib for _, lib in loader.missing_libs})
        print(f"note: some releases were missing libraries: {', '.join(names)}")


if __name__ == "__main__":
    parser = setup_parser("libmatch")
    main(parser.parse_args())
