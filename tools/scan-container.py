#!/usr/bin/env python3
"""Account for every sector of a container: what each one holds, and what is left.

`tools/scan-img.py` finds the texture/palette blobs and `tools/decomp/codemap.py`
finds the code.  Neither answers "what is the other 60% of TOWN.BIN", because
neither is told to be exhaustive.  This is the accounting pass: it runs both,
adds the formats they do not look for, classifies the residue, and prints a map
in which every sector is spoken for.

The classes, strongest evidence first:

| class  | how it is recognised | exact? |
|--------|----------------------|--------|
| `OVL`  | a code overlay already mapped in `splat/*.yaml` (the table below) | yes |
| `VAB`  | PSY-Q sound bank: `pBAV` magic, and the header's `fsize` is confirmed against the sum of the VAG size table | yes |
| `SEQ`  | KCET/TSDB/KDT sequence container: `KCET` magic plus the chunk's own length | yes |
| `IMG`  | an image blob, parsed by `tools/scan-img.py`'s own record walker | yes |
| `CODE` | MIPS code, located by `tools/decomp/codemap.py`'s function-start test | sector |
| `FILL` | incompressible mastered filler -- high per-sector entropy, almost no zero bytes, and zlib/xz expand it | sector |
| `ZERO` | every byte zero | sector |
| `TEXT` | Shift-JIS text, by two-byte-sequence coverage | sector |
| `PCM`  | SPU ADPCM, by the 16-byte block flag byte -- a VAB body whose header is elsewhere | sector |
| `DATA` | none of the above: structured bytes with no format recognised here | sector |

The first four are byte-exact and are claimed first, because each one states
its own length; the rest are sector-granular and only get the sectors the
exact classes left.  `DATA` is the honest residue -- read it as "this tool
does not know", not as "this is not an asset".

    python3 tools/scan-container.py town.bin
    python3 tools/scan-container.py town.bin --anchors       # the inventory
    python3 tools/scan-container.py town.bin --symbols s.txt # the name tables
    python3 tools/scan-container.py town.bin --json m.json
    python3 tools/scan-container.py data/town/town.bin --no-code

`--no-code` skips the codemap sweep, which is the slow pass (about a minute on
TOWN.BIN).  The image sweep needs no disc image: it reads the container's own
sectors through a shim with the same interface `scan-img.py`'s `Disc` has, so
the LBAs it reports are container-relative (`offset / 0x800`) and not disc
LBAs.
"""

import argparse
import collections
import dataclasses
import importlib.util
import json
import lzma
import pathlib
import struct
import sys
import zlib
from typing import Optional

import numpy as np

HERE = pathlib.Path(__file__).resolve().parent
ROOT = HERE.parent
SECTOR = 0x800

# The code segments already mapped in splat/*.yaml: (file_start, file_end,
# name, vram).  These are claimed outright -- the point of the map is what is
# NOT one of these.
#
# The three entries at 0x80088760 are the whole read the overlay table at
# vram 0x80080C38 asks for, not just the part splat has split: that table's
# descriptors are {nsec << 23 | dest, LBA} and say 278 sectors from LBA 3099
# (town.bin+0xb000), 187 from LBA 12636 (dungeon.bin+0xa3000) and 14 from LBA
# 1884 (main.bin+0x21800).  So each of those overlays carries DATA past the
# text splat ends at, and town's and dungeon's ends below are where the read
# ends, not where the text does.
DECLARED: dict[str, list[tuple[int, int, str, int]]] = {
    "town.bin": [
        (0xB000, 0x51FC0, "ovl_town text and rodata", 0x80088760),
        (0x51FC0, 0x95AC4, "ovl_town data", 0x800CF720),
    ],
    "dungeon.bin": [
        (0xA3000, 0xF7700, "ovl_dungeon text and rodata", 0x80088760),
        (0xF7700, 0x100800, "ovl_dungeon data", 0x800DCE60),
    ],
    "main.bin": [
        (0x21800, 0x28420, "ovl_main_80088760", 0x80088760),
        (0x25D000, 0x269000, "ovl_main_80020000", 0x80020000),
        (0x269000, 0x275800, "ovl_main_80400000", 0x80400000),
    ],
}

CONTAINERS = {
    "town.bin": "data/town/town.bin",
    "dungeon.bin": "data/dungeon/dungeon.bin",
    "main.bin": "data/main/main.bin",
    "slus_006.14": "data/slus_006.14",
}


# --------------------------------------------------------------------------
# borrowing the two existing tools
# --------------------------------------------------------------------------


def _load(path: pathlib.Path, name: str):
    """Import a module by path (the hyphens in tools/*.py stop a plain import)."""
    spec = importlib.util.spec_from_file_location(name, path)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"cannot load {path}")
    mod = importlib.util.module_from_spec(spec)
    sys.modules[name] = mod
    spec.loader.exec_module(mod)
    return mod


class ContainerDisc:
    """`scan-img.py`'s `Disc` interface over a flat container.

    Same methods, same semantics, with "LBA" meaning the container's own
    sector index.  `scan-img.py` only ever calls `sector`, `read` and
    `scan_heads`, so this is enough to run its sweep without az.bin.
    """

    def __init__(self, data: bytes):
        self.data = data
        self.sectors = len(data) // SECTOR

    def sector(self, lba: int) -> Optional[bytes]:
        if not 0 <= lba < self.sectors:
            return None
        return self.data[lba * SECTOR : (lba + 1) * SECTOR]

    def read(self, lba: int, nsec: int) -> Optional[bytes]:
        if lba < 0 or lba + nsec > self.sectors:
            return None
        return self.data[lba * SECTOR : (lba + nsec) * SECTOR]

    def scan_heads(self, start: int, stop: int, chunk: int = 512):
        for lba in range(max(0, start), min(stop, self.sectors)):
            ty, _size = struct.unpack_from("<HH", self.data, lba * SECTOR)
            if 1 <= ty <= 9:
                yield lba


# --------------------------------------------------------------------------
# regions
# --------------------------------------------------------------------------


@dataclasses.dataclass
class Region:
    off: int
    length: int
    kind: str
    detail: str = ""

    @property
    def end(self) -> int:
        return self.off + self.length


class Map:
    """Sector ownership plus the byte-exact anchor list."""

    def __init__(self, data: bytes):
        self.data = data
        self.nsec = len(data) // SECTOR
        self.owner: list[str] = [""] * self.nsec
        self.anchors: list[Region] = []

    def claim_partial(self, reg: Region) -> None:
        """Claim the free sectors a region covers; record it either way.

        For the parts of one object that share a sector with each other -- an
        overlay's text and the data that follows it -- which is not a
        collision worth reporting.
        """
        a = reg.off // SECTOR
        b = min(self.nsec, (reg.end + SECTOR - 1) // SECTOR)
        for i in range(a, b):
            if not self.owner[i]:
                self.owner[i] = reg.kind
        self.anchors.append(reg)

    def claim(self, reg: Region, exact: bool = True) -> bool:
        """Mark the sectors a region touches.  False if it collides."""
        a = reg.off // SECTOR
        b = min(self.nsec, (reg.end + SECTOR - 1) // SECTOR)
        if any(self.owner[i] for i in range(a, b)):
            return False
        for i in range(a, b):
            self.owner[i] = reg.kind
        if exact:
            self.anchors.append(reg)
        return True

    def free(self, a: int, b: int) -> bool:
        return not any(self.owner[i] for i in range(a, min(b, self.nsec)))


# --------------------------------------------------------------------------
# the exact formats
# --------------------------------------------------------------------------


def find_vabs(data: bytes) -> list[Region]:
    """PSY-Q sound banks.

    The header's `fsize` is checked against 32 + 2048 + 512 * programs + 512
    plus the sum of the 256-entry VAG size table (which is in 8-byte units).
    That equality holds to the byte on every bank in TOWN.BIN, so a bank whose
    stored bytes fall short of `fsize` is reported as truncated rather than
    silently trusted.
    """
    out = []
    for m in _finditer(data, b"pBAV"):
        if m % 4:
            continue
        if m + 32 > len(data):
            continue
        ver, _vid, fsize = struct.unpack_from("<III", data, m + 4)
        _res0, nprog, ntone, nvag = struct.unpack_from("<HHHH", data, m + 16)
        if not 1 <= ver <= 0x10 or not 1 <= nprog <= 128 or nvag > 254:
            continue
        hdr = 32 + 2048 + 512 * nprog + 512
        if m + hdr > len(data):
            continue
        tbl = struct.unpack_from("<256H", data, m + 32 + 2048 + 512 * nprog)
        body = sum(s << 3 for s in tbl)
        if hdr + body != fsize:
            continue
        stored = min(fsize, len(data) - m)
        detail = f"{nprog} program{'s' * (nprog != 1)}, {ntone} tones, {nvag} VAGs, header {hdr:#x}"
        if stored != fsize:
            detail += f" -- TRUNCATED, {stored:#x} of {fsize:#x} bytes present"
        out.append(Region(m, stored, "VAB", detail))
    return out


def find_seqs(data: bytes) -> list[Region]:
    """KCET sequence containers: `KCET len` wrapping `TSDB` / `KDT2` / `KDT `.

    `tools/kdt-render.py` already reads one of these; all this adds is the
    outer chunk's length, which is what makes the extent exact.
    """
    out = []
    for m in _finditer(data, b"KCET"):
        if m + 12 > len(data):
            continue
        (size,) = struct.unpack_from("<I", data, m + 4)
        if not 0x40 <= size <= len(data) - m - 8:
            continue
        if data[m + 8 : m + 12] != b"TSDB":
            continue
        # KCET / TSDB / KDT2 / KDT, the last at +0x24 -- the offset
        # tools/kdt-render.py's find_kdt() already uses.  Its +0xC is the
        # track count.
        ntrk = ""
        if data[m + 0x24 : m + 0x28] == b"KDT ":
            ntrk = f", {struct.unpack_from('<I', data, m + 0x30)[0]} tracks"
        out.append(Region(m, 8 + size, "SEQ", f"KCET/TSDB{ntrk}"))
    return out


SYM_STRIDE = 36


def _sym_at(data: bytes, off: int) -> Optional[tuple[int, str]]:
    """One `struct { char name[32]; s32 value; }`, or None.

    THE NAME COMES FIRST.  Reading it the other way round costs nothing on a
    single record -- the stride and the NUL padding look the same -- but it
    pairs every name with its PREDECESSOR's value, which is off by one
    everywhere and wrong by a whole group at every group boundary.  Two
    independent checks fix the order: `snf_false` is 0 and `snf_truth` is 1
    only in this order, and `DFLT_END_PSN_DM` is 22 only in this order, which
    is the index `ovl_town`'s own dispatcher starts the per-actor table at
    (`-0x58` = `-22 * 4`).  See docs/town-identifiers.md.
    """
    if off < 0 or off + SYM_STRIDE > len(data):
        return None
    nm = data[off : off + 32]
    k = nm.find(b"\0")
    if k <= 0 or any(nm[k:]):
        return None
    name = nm[:k]
    if not all(0x21 <= c <= 0x7E for c in name):
        return None
    (val,) = struct.unpack_from("<i", data, off + 32)
    return val, name.decode()


def find_symbol_tables(data: bytes, least: int = 8) -> list[tuple[Region, list[tuple[int, str]]]]:
    """The scenario system's own identifier tables.

    `struct { char name[32]; s32 value; }` repeated, NUL-padded, every byte
    after the terminator zero.  Two of them are in TOWN.BIN and they carry
    3,587 of the original source's names -- scenario flags, callbacks, image
    ids, item ids.  Nothing else in the four containers matches the shape,
    which is why 8 consecutive records is a safe floor: random data cannot
    produce one record, let alone eight in a row at a fixed stride.
    """
    out = []
    off = 0
    while off < len(data) - SYM_STRIDE:
        if _sym_at(data, off) is None:
            off += 4
            continue
        rows = []
        p = off
        while True:
            r = _sym_at(data, p)
            if r is None:
                break
            rows.append(r)
            p += SYM_STRIDE
        if len(rows) >= least:
            out.append(
                (Region(off, p - off, "SYMS", f"{len(rows)} name/value records"), rows)
            )
            off = p
        else:
            off += 4
    return out


def find_images(data: bytes, quiet: bool) -> list[Region]:
    """Delegate to scan-img.py's own sweep over the container's sectors."""
    si = _load(HERE / "scan-img.py", "scan_img")
    mk = si._load_make_img()
    disc = ContainerDisc(data)
    descs = si.sweep(mk, disc, 0, disc.sectors, 1024, set(), progress=not quiet)
    out = []
    for d in descs:
        tex = sum(1 for r in d.records if r.type in si.TEX_TYPES)
        clut = sum(1 for r in d.records if r.type in si.CLUT_TYPES)
        out.append(
            Region(
                d.lba * SECTOR,
                d.payload_bytes,
                "IMG",
                f"{si.blob_tag(d.records)}: {len(d.records)} records, "
                f"{tex} textures, {clut} palettes",
            )
        )
    return out


def find_code(path: pathlib.Path, window: int) -> list[Region]:
    """codemap.py's function-start sweep, grouped into runs of constant delta.

    A hit is a window, so the extent is sector-granular: `[first hit, last hit
    + window)`, clipped to the sectors the exact formats did not claim.  Use
    `codemap.py --pin` to put a real boundary on anything this reports.
    """
    cm = _load(ROOT / "tools/decomp/codemap.py", "codemap")
    img = cm.Image(str(path))
    hits = img.sweep(window, 10, 0.90)
    runs = []
    for lo, d, score, ntgt, npro in hits:
        if runs and runs[-1][1] == d and lo <= runs[-1][2] + window:
            runs[-1][2] = lo
            runs[-1][3] = max(runs[-1][3], score)
            runs[-1][4] += 1
        else:
            runs.append([lo, d, lo, score, 1])
    out = []
    for first, d, last, score, n in runs:
        vram = (first + d) & 0xFFFFFFFF
        out.append(
            Region(
                first,
                last + window - first,
                "CODE",
                f"loads at {vram:#010x} (delta {d:#x}), {n} window hits, peak {score} targets",
            )
        )
    return out


def _finditer(data: bytes, needle: bytes):
    i = data.find(needle)
    while i >= 0:
        yield i
        i = data.find(needle, i + 1)


# --------------------------------------------------------------------------
# the residue
# --------------------------------------------------------------------------

def _sjis_sector(sec: bytes) -> tuple[float, float, int]:
    """(pair coverage, half-width-katakana fraction, spaces + newlines).

    The walk is NON-overlapping, because that is the only way to tell text
    from binary that happens to contain lead bytes: a real Shift-JIS stream
    stays in phase from the first byte to the last.

    The other two numbers are what stop a dense binary table from passing.
    This script is written in full-width characters -- the ideographic space
    0x8140 alone is a fifth of it -- and uses `\\n` for line breaks, while the
    single-byte half-width katakana at 0xA1..0xDF never appear in it at all.
    """
    i = 0
    cov = han = mark = 0
    n = len(sec)
    while i < n - 1:
        b = sec[i]
        if 0x81 <= b <= 0x9F or 0xE0 <= b <= 0xEF:
            t = sec[i + 1]
            if 0x40 <= t <= 0xFC and t != 0x7F:
                cov += 2
                if b == 0x81 and t == 0x40:
                    mark += 1
                i += 2
                continue
        if 0xA1 <= b <= 0xDF:
            han += 1
        elif b == 0x0A:
            mark += 1
        i += 1
    return cov / n, han / n, mark


def sector_stats(data: bytes, nsec: int):
    a = np.frombuffer(data[: nsec * SECTOR], dtype=np.uint8).reshape(nsec, SECTOR)
    zero = (a == 0).mean(axis=1)
    cnt = np.zeros((nsec, 256), dtype=np.int64)
    for i in range(nsec):
        cnt[i] = np.bincount(a[i], minlength=256)
    p = cnt / SECTOR
    lg = np.zeros_like(p)
    np.log2(p, out=lg, where=p > 0)
    ent = -(p * lg).sum(axis=1)
    # SPU ADPCM: every 16-byte block's second byte is a flag in 0..7 and its
    # first byte's high nibble (the filter) is 0..4.
    blk = a.reshape(nsec, SECTOR // 16, 16)
    pcm = ((blk[:, :, 1] < 8) & ((blk[:, :, 0] >> 4) < 5)).mean(axis=1)
    return ent, zero, pcm


def classify_residue(data: bytes, mp: Map, ent, zero, pcm) -> None:
    """Label the sectors the exact formats and the code sweep left over."""
    free = [i for i in range(mp.nsec) if not mp.owner[i]]
    fillish = [i for i in free if ent[i] > 7.85 and zero[i] < 0.02]
    # Filler is a property of a RUN, not a sector: a lone high-entropy sector
    # is far more likely to be compressed payload.  Require 16 in a row and
    # then confirm the run really is incompressible, which is the test
    # docs/main-bin-data.md closed that container's dead region with.
    filler = set()
    for a, b in _runs(fillish):
        if b - a < 16:
            continue
        probe = data[a * SECTOR : min(b * SECTOR, a * SECTOR + (1 << 20))]
        if len(zlib.compress(probe, 9)) < len(probe) or len(lzma.compress(probe)) < len(probe):
            continue
        filler.update(range(a, b))
        lo, hi = _grow_filler(data, mp, a, b)
        raw = data[lo:hi]
        cnt = np.bincount(np.frombuffer(raw, dtype=np.uint8), minlength=256)
        p = cnt / len(raw)
        h = float(-(p[p > 0] * np.log2(p[p > 0])).sum())
        chi = float(((cnt - len(raw) / 256) ** 2 / (len(raw) / 256)).sum())
        mp.anchors.append(
            Region(lo, hi - lo, "FILL",
                   f"incompressible filler: entropy {h:.4f}, zero fraction "
                   f"{cnt[0] / len(raw):.5f}, chi-square {chi:.1f}, zlib and xz both expand it"))
    for i in free:
        if zero[i] == 1.0:
            mp.owner[i] = "ZERO"
            continue
        if i in filler:
            mp.owner[i] = "FILL"
            continue
        cov, han, mark = _sjis_sector(data[i * SECTOR : (i + 1) * SECTOR])
        if cov >= 0.60 and han < 0.05 and (mark >= 20 or (cov >= 0.80 and mark >= 6)):
            mp.owner[i] = "TEXT"
        elif pcm[i] > 0.97 and ent[i] > 6.5:
            mp.owner[i] = "PCM"
        else:
            mp.owner[i] = "DATA"


def _grow_filler(data: bytes, mp: Map, a: int, b: int) -> tuple[int, int]:
    """Byte-exact ends of a filler run, which starts and ends mid-sector.

    Filler has no 16-byte zero run in it (the odds are 2**-128 per position),
    so the last one below the run and the first one above it bracket it
    exactly.  Only unclaimed sectors are searched: a neighbour that belongs to
    a real segment ends the run at its own boundary.
    """
    lo, hi = a * SECTOR, b * SECTOR
    if a > 0 and not mp.owner[a - 1]:
        prev = data[(a - 1) * SECTOR : lo]
        k = prev.rfind(b"\x00" * 16)
        if k >= 0:
            while k < len(prev) and prev[k] == 0:
                k += 1
            lo = (a - 1) * SECTOR + k
    if b < mp.nsec and not mp.owner[b]:
        nxt = data[hi : (b + 1) * SECTOR]
        k = nxt.find(b"\x00" * 16)
        if k >= 0:
            hi += k
    return lo, hi


def _runs(idx) -> list[tuple[int, int]]:
    out = []
    for i in idx:
        if out and out[-1][1] == i:
            out[-1][1] = i + 1
        else:
            out.append([i, i + 1])
    return [(a, b) for a, b in out]


# --------------------------------------------------------------------------
# reporting
# --------------------------------------------------------------------------

ORDER = ["OVL", "CODE", "IMG", "VAB", "SEQ", "SYMS", "PCM", "TEXT", "DATA", "FILL", "ZERO"]


def sector_runs(mp: Map) -> list[tuple[int, int, str]]:
    out: list[list] = []
    for i, k in enumerate(mp.owner):
        if out and out[-1][2] == k and out[-1][1] == i:
            out[-1][1] = i + 1
        else:
            out.append([i, i + 1, k])
    return [(a, b, k) for a, b, k in out]


def main() -> None:
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    ap.add_argument("container", help="a name from the table, or a path")
    ap.add_argument("--anchors", action="store_true", help="list every byte-exact region")
    ap.add_argument("--no-code", action="store_true", help="skip the codemap sweep")
    ap.add_argument("--window", type=lambda s: int(s, 0), default=0x6000,
                    help="codemap sweep window (default 0x6000)")
    ap.add_argument("--min-run", type=int, default=1,
                    help="only print sector runs of at least this many sectors")
    ap.add_argument("--json", metavar="FILE")
    ap.add_argument("--symbols", metavar="FILE",
                    help="write every symbol table's name/value rows here")
    ap.add_argument("--quiet", action="store_true", help="summary only")
    args = ap.parse_args()

    rel = CONTAINERS.get(args.container) or str(args.container)
    path = pathlib.Path(rel)
    if not path.is_absolute():
        path = ROOT / rel
    data = path.read_bytes()
    name = path.name
    mp = Map(data)
    print(f"{path.relative_to(ROOT) if path.is_relative_to(ROOT) else path}  "
          f"{len(data)} bytes ({len(data):#x}), {mp.nsec} sectors")

    for lo, hi, nm, vram in DECLARED.get(name, []):
        mp.claim_partial(Region(lo, hi - lo, "OVL", f"{nm}, loads at {vram:#010x}"))

    symtabs = find_symbol_tables(data)
    for reg, _rows in symtabs:
        # Two tables can share the sector their boundary falls in, which is
        # not a collision worth reporting.
        mp.claim_partial(reg)

    collisions = []
    for finder in (find_vabs(data), find_seqs(data), find_images(data, args.quiet)):
        for reg in sorted(finder, key=lambda r: r.off):
            if mp.claim(reg):
                continue
            # A region that declares more bytes than the container gives it:
            # keep the sectors it really owns and say so, rather than dropping
            # it or letting it swallow the segment it runs into.
            a = reg.off // SECTOR
            b = a
            while b < mp.nsec and not mp.owner[b]:
                b += 1
            if b > a:
                clipped = Region(
                    reg.off,
                    b * SECTOR - reg.off,
                    reg.kind,
                    reg.detail
                    + f" -- CUT at {b * SECTOR:#x} by the next segment, "
                    f"{b * SECTOR - reg.off:#x} of {reg.length:#x} bytes present",
                )
                mp.claim(clipped)
            else:
                collisions.append(reg)

    if not args.no_code:
        for reg in find_code(path, args.window):
            a, b = reg.off // SECTOR, (reg.end + SECTOR - 1) // SECTOR
            # Code hits are windows and may overlap an exact region; give the
            # exact one priority and keep only the free sectors.
            for ca, cb in _runs([i for i in range(a, min(b, mp.nsec)) if not mp.owner[i]]):
                mp.claim(Region(ca * SECTOR, (cb - ca) * SECTOR, "CODE", reg.detail),
                         exact=False)

    ent, zero, pcm = sector_stats(data, mp.nsec)
    classify_residue(data, mp, ent, zero, pcm)

    if args.anchors:
        print("\nANCHORS -- byte-exact, each region states its own length")
        print(f"  {'file':>9}..{'end':<9} {'bytes':>9}  kind  detail")
        for r in sorted(mp.anchors, key=lambda r: r.off):
            print(f"  {r.off:#09x}..{r.end:#09x} {r.length:>9}  {r.kind:4}  {r.detail}")

    if not args.quiet:
        print("\nSECTOR MAP")
        for a, b, k in sector_runs(mp):
            if b - a < args.min_run:
                continue
            print(f"  {a * SECTOR:#09x}..{b * SECTOR:#09x}  {b - a:>5} sec  {k}")

    print("\nSUMMARY")
    cnt = collections.Counter(mp.owner)
    for k in ORDER + sorted(set(cnt) - set(ORDER)):
        if not cnt.get(k):
            continue
        n = cnt[k]
        print(f"  {k:5} {n:>5} sectors  {n * SECTOR:>9} bytes  {n / mp.nsec:6.2%}")
    print(f"  {'total':5} {mp.nsec:>5} sectors  {mp.nsec * SECTOR:>9} bytes")
    if collisions:
        print(f"\n{len(collisions)} exact regions collided with an earlier claim:")
        for r in collisions:
            print(f"  {r.off:#09x}..{r.end:#09x}  {r.kind}  {r.detail}")

    if args.symbols:
        with open(args.symbols, "w") as fh:
            for reg, rows in symtabs:
                fh.write(f"# {reg.off:#09x}..{reg.end:#09x}  {len(rows)} records\n")
                for val, nm in rows:
                    fh.write(f"{val:<8} {nm}\n")
        print(f"wrote {sum(len(r) for _, r in symtabs)} symbols to {args.symbols}",
              file=sys.stderr)

    if args.json:
        out = {
            "container": str(path),
            "bytes": len(data),
            "anchors": [dataclasses.asdict(r) for r in sorted(mp.anchors, key=lambda r: r.off)],
            "sectors": [
                {"off": a * SECTOR, "end": b * SECTOR, "sectors": b - a, "kind": k}
                for a, b, k in sector_runs(mp)
            ],
            "summary": {k: v for k, v in cnt.items()},
        }
        pathlib.Path(args.json).write_text(json.dumps(out, indent=1))
        print(f"wrote {args.json}", file=sys.stderr)


if __name__ == "__main__":
    main()
