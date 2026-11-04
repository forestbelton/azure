#!/usr/bin/env python3
"""Find the engine's asset blobs on the disc and decode their record streams.

The CD queue (``func_8003E4FC`` kind 6, pumped in ``func_8003E758``) takes
8-byte descriptors ``{nsec << 23 | dest, LBA}``, where ``dest == 0`` means "the
D_80081480 bump arena".  Once a blob is in RAM, ``func_80041344``
(``csrc/unk_14934.c``) or its async sibling ``func_80041588`` walks it as a
stream of records::

    u16 type        1..9; 0 or >9 ends the stream (`default: return`)
    u16 size        the record's own byte size; 0 ends the stream
    u32 w0, w1, w2  meaning depends on type

This script scans the resident executable (and, optionally, the code overlays)
for descriptor-shaped word pairs, pulls each candidate's sectors out of the raw
disc image, and keeps the ones whose payload parses as a record stream.  The
result is a manifest of every texture and palette the game loads, with the VRAM
rectangle each one lands on.

Typical use::

    uv run python tools/scan-img.py                       # manifest to stdout
    uv run python tools/scan-img.py --json out.json
    uv run python tools/scan-img.py --extract blobs/      # raw blobs
    uv run python tools/scan-img.py --png pngs/           # decoded textures
    uv run python tools/scan-img.py --sweep               # + the whole disc
    uv run python tools/scan-img.py --lba 0x3154 --nsec 8 # one blob, no scan

The executable only holds the descriptors for the boot and UI art; the engine
builds the rest at runtime from a container base plus an offset, so --sweep is
what reaches the town and dungeon art.  It roughly doubles the texture count
and takes about ten seconds over the whole disc.

Neither colour depth nor palette is recorded anywhere in a blob -- the game
knows both from the drawing primitive's tpage and CLUT id -- so --depth and
--palette are guesses.  4bpp is right for most things; the title background at
lba 0xb66 is 8bpp.
"""

import argparse
import dataclasses
import importlib.util
import io
import json
import os
import pathlib
import re
import struct
import sys
from typing import Iterator, Optional

HERE = pathlib.Path(__file__).resolve().parent
ROOT = HERE.parent

SECTOR_SIZE = 2352
SECTOR_DATA = 2048
SYNC_PATTERN = bytes([0x00] + [0xFF] * 10 + [0x00])

# VRAM is 1024x512 16-bit words.
VRAM_W = 1024
VRAM_H = 512

K0BASE = 0x80000000
RAM_SIZE = 0x200000

# Record types, keyed by the on-disc `u16 type`.  `arm` is the `case N:` label
# in func_80041344, where N == type - 1.
TYPES = {
    1: ("TEX_LZ", "case 0", "texture, LZ-compressed into the work buffer"),
    2: ("CLUT", "case 1", "palette, flags used verbatim"),
    3: ("RAM_LZ", "case 2", "payload, LZ-compressed to RAM (not VRAM)"),
    4: ("RAM_COPY", "case 3", "payload, raw memcpy to RAM (not VRAM)"),
    5: ("TEX_RAW", "case 4", "texture, uncompressed, straight to VRAM"),
    6: ("CLUT_STP", "case 5", "palette, flags |= 2"),
    7: ("CLUT_STP_ALL", "case 6", "palette, STP set on every entry but 0"),
    8: ("CLUT_STP16", "case 7", "palette, STP set on 1..15 of each 16"),
    9: ("RELOC", "case 8", "pointer-table fixup, not image data"),
}

# How many bytes of its record each arm actually reads.  This is not uniform:
# the type-3 arm stops at unk_08, so its records are 12 bytes, and rejecting
# anything under 16 drops the whole compressed-payload family.
FIELD_BYTES = {1: 16, 2: 14, 3: 12, 4: 16, 5: 16, 6: 14, 7: 14, 8: 14, 9: 10}

TEX_TYPES = (1, 5)
CLUT_TYPES = (2, 6, 7, 8)
RAM_TYPES = (3, 4)

# Default images to scan: the resident executable plus the two code overlays
# (file offsets and vram bases from docs/overlays.md).
DEFAULT_IMAGES = [
    ("slus_006.14", "data/slus_006.14", 0x800, 0x8002D000, 0x54800),
    ("town.bin", "data/town/town.bin", 0xB000, 0x80088760, 0x46FC0),
    ("dungeon.bin", "data/dungeon/dungeon.bin", 0xA3000, 0x80088760, 0x54700),
]


# --------------------------------------------------------------------------
# make-img.py reuse
# --------------------------------------------------------------------------


def _load_make_img():
    """Import tools/make-img.py (the hyphen stops a plain import)."""
    spec = importlib.util.spec_from_file_location("make_img", HERE / "make-img.py")
    if spec is None or spec.loader is None:
        raise RuntimeError("cannot load tools/make-img.py")
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


# --------------------------------------------------------------------------
# disc access
# --------------------------------------------------------------------------


class Disc:
    """Random sector access into a MODE2/2352 raw image, indexed by LBA."""

    def __init__(self, path: pathlib.Path):
        self.path = path
        self.f = open(path, "rb")
        size = self.f.seek(0, os.SEEK_END)
        if size % SECTOR_SIZE != 0:
            raise SystemExit(
                f"{path}: {size} bytes is not a multiple of {SECTOR_SIZE}; "
                "this does not look like a MODE2/2352 image"
            )
        self.sectors = size // SECTOR_SIZE
        self._cache: dict[int, Optional[bytes]] = {}

    def sector(self, lba: int) -> Optional[bytes]:
        """2048 bytes of user data, or None if the sector is unusable."""
        if lba in self._cache:
            return self._cache[lba]
        out: Optional[bytes] = None
        if 0 <= lba < self.sectors:
            self.f.seek(lba * SECTOR_SIZE)
            raw = self.f.read(SECTOR_SIZE)
            if len(raw) == SECTOR_SIZE and raw[:12] == SYNC_PATTERN and raw[15] == 2:
                sub = raw[16:20]
                # form 2 sectors carry 2324 bytes and no 2048-byte user area
                if sub == raw[20:24] and not (sub[2] & (1 << 5)):
                    out = raw[24 : 24 + SECTOR_DATA]
        if len(self._cache) > 4096:
            self._cache.clear()
        self._cache[lba] = out
        return out

    def read(self, lba: int, nsec: int) -> Optional[bytes]:
        buf = bytearray()
        for i in range(nsec):
            s = self.sector(lba + i)
            if s is None:
                return None
            buf += s
        return bytes(buf)

    def scan_heads(self, start: int, stop: int, chunk: int = 512) -> Iterator[int]:
        """LBAs in [start, stop) whose first 4 bytes could start a record.

        Read sequentially in big chunks: the per-sector seek is what makes a
        whole-disc sweep slow, and almost every sector fails this test.
        """
        lba = start
        while lba < stop:
            n = min(chunk, stop - lba)
            self.f.seek(lba * SECTOR_SIZE)
            raw = self.f.read(n * SECTOR_SIZE)
            for i in range(len(raw) // SECTOR_SIZE):
                base = i * SECTOR_SIZE
                if raw[base : base + 12] != SYNC_PATTERN or raw[base + 15] != 2:
                    continue
                sub1 = raw[base + 16 : base + 20]
                if sub1 != raw[base + 20 : base + 24] or (sub1[2] & (1 << 5)):
                    continue
                ty, size = struct.unpack_from("<HH", raw, base + 24)
                if not 1 <= ty <= 9:
                    continue
                if size != 0 and size < FIELD_BYTES[ty]:
                    continue
                yield lba + i
            lba += n


class DiscStream:
    """A file-like window into the blob at `lba`, starting `off` bytes in.

    Handed to make-img.py's decompress(), which reads lazily; `consumed`
    afterwards is the compressed payload's real length, which is how the disc
    sweep learns where a blob ends.
    """

    def __init__(self, disc: Disc, lba: int, off: int, cap_sectors: int):
        self.disc = disc
        self.lba = lba
        self.pos = off
        self.limit = cap_sectors * SECTOR_DATA
        self.consumed = off

    def read(self, n: int) -> bytes:
        out = bytearray()
        while n > 0:
            if self.pos >= self.limit:
                break
            sec = self.disc.sector(self.lba + self.pos // SECTOR_DATA)
            if sec is None:
                break
            k = self.pos % SECTOR_DATA
            take = min(n, SECTOR_DATA - k)
            out += sec[k : k + take]
            self.pos += take
            n -= take
        self.consumed = max(self.consumed, self.pos)
        return bytes(out)


# --------------------------------------------------------------------------
# images (the exe and the overlays), and the symbol map used to name things
# --------------------------------------------------------------------------


@dataclasses.dataclass
class Image:
    name: str
    data: bytes  # just the mapped window
    file_off: int  # where the window starts in the container
    vram: int  # vram address of data[0]

    def __len__(self) -> int:
        return len(self.data)

    def contains_vram(self, addr: int) -> bool:
        return self.vram <= addr < self.vram + len(self.data)

    def word(self, index: int) -> int:
        return struct.unpack_from("<I", self.data, index * 4)[0]

    def vram_of(self, off: int) -> int:
        return self.vram + off

    def file_of(self, off: int) -> int:
        return self.file_off + off


def load_images(specs: list[tuple[str, str, int, int, int]]) -> list[Image]:
    images = []
    for name, rel, file_off, vram, length in specs:
        path = ROOT / rel
        if not path.exists():
            print(f"warning: {rel} not found, skipping", file=sys.stderr)
            continue
        with open(path, "rb") as f:
            f.seek(file_off)
            data = f.read(length)
        if len(data) < 8:
            print(f"warning: {rel} too short at {file_off:#x}, skipping", file=sys.stderr)
            continue
        images.append(Image(name=name, data=data, file_off=file_off, vram=vram))
    return images


DLABEL_RE = re.compile(r"^dlabel\s+(\S+)\s*$")
DATALINE_RE = re.compile(r"^\s*/\*\s*([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\s")


def load_symbols() -> list[tuple[int, str]]:
    """(vram, name) for every dlabel in asm/data, sorted by address.

    Used only to name the place a descriptor is referenced from, so a partial
    map is fine; data that has already migrated into csrc just goes unnamed.
    """
    syms: dict[int, str] = {}
    for path in sorted((ROOT / "asm" / "data").rglob("*.s")):
        pending = None
        try:
            text = path.read_text(errors="replace")
        except OSError:
            continue
        for line in text.splitlines():
            m = DLABEL_RE.match(line)
            if m:
                pending = m.group(1)
                continue
            if pending:
                m = DATALINE_RE.match(line)
                if m:
                    syms.setdefault(int(m.group(2), 16), pending)
                    pending = None
    return sorted(syms.items())


def name_for(syms: list[tuple[int, str]], addr: int) -> str:
    """`sym+0x8` / `sym[2]` for the nearest preceding dlabel."""
    lo, hi = 0, len(syms)
    while lo < hi:
        mid = (lo + hi) // 2
        if syms[mid][0] <= addr:
            lo = mid + 1
        else:
            hi = mid
    if lo == 0:
        return f"{addr:#010x}"
    base, nm = syms[lo - 1]
    delta = addr - base
    if delta == 0:
        return nm
    if delta % 4 == 0:
        return f"{nm}[{delta // 4}]"
    return f"{nm}+{delta:#x}"


# --------------------------------------------------------------------------
# record stream
# --------------------------------------------------------------------------


@dataclasses.dataclass
class Record:
    offset: int  # offset of the record header within the blob
    type: int
    size: int
    raw: bytes  # the record's own bytes, up to 16
    warn: list[str] = dataclasses.field(default_factory=list)
    packed: Optional[int] = None  # compressed payload length, once measured
    unpacked: Optional[int] = None
    rows: Optional[int] = None  # rows actually supplied, if fewer than h

    @property
    def kind(self) -> str:
        return TYPES[self.type][0]

    # Field accessors mirror how the arms read the record: halfwords through
    # the `u8 *` cursor, words through the Unk41344 pointer.
    def u16(self, at: int) -> int:
        return struct.unpack_from("<H", self.raw, at)[0]

    def u32(self, at: int) -> int:
        return struct.unpack_from("<I", self.raw, at)[0]

    @property
    def data_off(self) -> int:
        return self.u32(4)

    # textures -------------------------------------------------------------
    @property
    def rect(self) -> tuple[int, int, int, int]:
        """(x, y, w, h); w is in 16-bit VRAM words, as LoadImage takes it."""
        return (self.u16(8), self.u16(0xA), self.u16(0xC), self.u16(0xE))

    # palettes -------------------------------------------------------------
    @property
    def clut_param(self) -> int:
        return self.u16(8)

    @property
    def clut_entries(self) -> int:
        """func_8003F80C's `arg2`: the palette count, of 16 colours each."""
        return self.u16(0xA)

    @property
    def clut_flags(self) -> int:
        base = self.u16(0xC)
        if self.type == 7:
            return base | 4  # func_80041344; the sibling ORs 2 here
        if self.type in (6, 8):
            return base | 2
        return base

    def clut_rect(self) -> tuple[int, int, int, int]:
        """func_8003F80C's RECT for this palette upload."""
        n = (self.clut_param - 0x7000) >> 6
        x = ((n & 0x40) << 1) + ((self.clut_param & 0x3F) << 4)
        y = (n & 0x3F) + 0x1C0
        return (x, y, self.clut_entries << 4, 1)

    # RAM payloads ---------------------------------------------------------
    @property
    def ram_off(self) -> int:
        """type 3: destination is D_80080A8C + this (func_80041588: absolute)."""
        return self.u32(8)

    @property
    def ram_len(self) -> int:
        """type 4, as func_80041588 reads it; func_80041344 copies len + dest."""
        return self.u32(0xC)

    @property
    def reloc_count(self) -> int:
        return self.u16(8)


class BadStream(Exception):
    pass


def parse_stream(blob: bytes, strict: bool = True,
                 payload_limit: Optional[int] = -1) -> list[Record]:
    """Walk a blob exactly as func_80041344 does.

    Raises BadStream when the blob is not a record stream.  The checks beyond
    the game's own are the point of the tool: random data almost never
    survives them, which is what makes the descriptor scan usable.
    """
    recs: list[Record] = []
    off = 0
    n = len(blob)
    if payload_limit == -1:
        payload_limit = n
    while True:
        if off + 4 > n:
            raise BadStream(f"record header at {off:#x} runs past the blob")
        ty, size = struct.unpack_from("<HH", blob, off)
        if ty == 0 or ty > 9:
            # func_80041344's `default: return` -- a clean end of stream
            break
        need = FIELD_BYTES[ty]
        if off + need > n:
            raise BadStream(
                f"record at {off:#x} (type {ty}) needs {need} bytes, "
                f"{n - off} left in the blob"
            )
        rec = Record(
            offset=off, type=ty, size=size, raw=blob[off : off + min(16, n - off)]
        )
        if strict:
            if size != 0 and size < need:
                raise BadStream(
                    f"record at {off:#x} (type {ty}) has size {size} < {need}"
                )
            check_record(rec, payload_limit)
        recs.append(rec)
        if size == 0:
            break
        off += size
    if strict and not recs:
        raise BadStream("no records")
    return recs


def check_record(rec: Record, blob_len: Optional[int]) -> None:
    """`blob_len` None checks only what the header alone can tell."""
    if blob_len is not None and rec.data_off >= blob_len:
        raise BadStream(
            f"record at {rec.offset:#x} ({rec.kind}) payload {rec.data_off:#x} "
            f"past blob end {blob_len:#x}"
        )
    if rec.type in TEX_TYPES:
        x, y, w, h = rec.rect
        if w == 0 or h == 0:
            raise BadStream(f"record at {rec.offset:#x} has an empty rect")
        if x + w > VRAM_W or y + h > VRAM_H:
            raise BadStream(
                f"record at {rec.offset:#x} rect ({x},{y},{w},{h}) leaves VRAM"
            )
        if rec.type == 5 and blob_len is not None:
            over = rec.data_off + w * h * 2 - blob_len
            if over > 0:
                # Real and in the game: two 64x128 records at lba 0x395c/0x3964
                # read 20 bytes past their own 8 sectors, so LoadImage picks up
                # whatever follows in the arena.  Not a reason to reject.
                rec.warn.append(f"raw texture reads {over} bytes past the blob")
    elif rec.type in CLUT_TYPES:
        if not 0x7000 <= rec.clut_param < 0x8000:
            raise BadStream(
                f"record at {rec.offset:#x} clut param {rec.clut_param:#x} "
                "outside 0x7000..0x7fff"
            )
        if rec.clut_entries == 0:
            raise BadStream(f"record at {rec.offset:#x} has 0 palettes")
        if blob_len is not None and rec.data_off + rec.clut_entries * 32 > blob_len:
            raise BadStream(f"record at {rec.offset:#x} palette overruns the blob")
        x, y, w, h = rec.clut_rect()
        if x + w > VRAM_W or not 0x1C0 <= y < VRAM_H:
            raise BadStream(
                f"record at {rec.offset:#x} clut rect ({x},{y},{w},{h}) leaves VRAM"
            )


# --------------------------------------------------------------------------
# descriptors
# --------------------------------------------------------------------------


def payload_end(rec: Record) -> Optional[int]:
    """Where a record's payload ends, when that is knowable without unpacking."""
    if rec.type == 5:
        _x, _y, w, h = rec.rect
        return rec.data_off + w * h * 2
    if rec.type in CLUT_TYPES:
        return rec.data_off + rec.clut_entries * 32
    if rec.type == 4:
        return rec.data_off + rec.ram_len
    if rec.type == 9:
        return rec.data_off + rec.reloc_count * 8
    return None  # types 1 and 3 are compressed; only unpacking tells


def resolve_payloads(mk, disc: Disc, lba: int, recs: list[Record],
                     cap_sectors: int) -> int:
    """Unpack every compressed record to find the blob's true end.

    Also the strongest validation the tool has: a type-1 record has to
    decompress to exactly its rectangle's byte count, which random data will
    not do.  Returns the blob's length in bytes.
    """
    end = 0
    for rec in recs:
        known = payload_end(rec)
        if known is not None:
            end = max(end, known)
            continue
        stream = DiscStream(disc, lba, rec.data_off, cap_sectors)
        try:
            data = mk.decompress(stream)
        except (AssertionError, IndexError, struct.error) as e:
            raise BadStream(f"record at {rec.offset:#x} will not unpack: {e}")
        if rec.type == 1:
            _x, _y, w, h = rec.rect
            stride = w * 2
            if stride == 0 or len(data) % stride != 0:
                raise BadStream(
                    f"record at {rec.offset:#x} unpacks to {len(data)} bytes, "
                    f"not a whole number of {stride}-byte rows"
                )
            rows = len(data) // stride
            if not 0 < rows <= h:
                raise BadStream(
                    f"record at {rec.offset:#x} unpacks to {rows} rows, "
                    f"record declares h={h}"
                )
            rec.rows = rows
            if rows != h:
                # Real, and not a misread: `h` is the VRAM slot's height, not
                # the record's.  A tile replaced frame by frame arrives as two
                # half-height records that overlap by exactly what each is
                # short, so the second LoadImage's data covers the first's
                # overshoot.  `rows` is the extractable height; see
                # docs/image-blobs.md.
                rec.warn.append(f"supplies {rows} rows, declares h={h}")
        rec.unpacked = len(data)
        rec.packed = stream.consumed - rec.data_off
        end = max(end, stream.consumed)
    return end


@dataclasses.dataclass
class Descriptor:
    lba: int
    nsec: int
    dest: int  # physical address, 0 == the D_80081480 bump arena
    sites: list[str] = dataclasses.field(default_factory=list)
    refs: list[str] = dataclasses.field(default_factory=list)
    records: list[Record] = dataclasses.field(default_factory=list)
    note: str = ""
    source: str = "descriptor"
    payload_bytes: Optional[int] = None

    @property
    def key(self) -> tuple[int, int]:
        return (self.lba, self.nsec)


def descriptor_candidates(img: Image, disc: Disc) -> Iterator[tuple[int, int, int, int]]:
    """(offset, lba, nsec, dest) for every descriptor-shaped word pair."""
    for off in range(0, len(img) - 8 + 1, 4):
        w0, w1 = struct.unpack_from("<II", img.data, off)
        nsec = w0 >> 23
        if nsec == 0:
            continue
        dest = w0 & 0x7FFFFF
        # dest is either the bump arena or a real RAM address; the low 2 MB
        # below 0x10000 is kernel/scratch and never a load target here
        if dest != 0 and not (0x10000 <= dest < RAM_SIZE):
            continue
        if not 1 <= w1 < disc.sectors:
            continue
        yield off, w1, nsec, dest


def crossref(images: list[Image], syms: list[tuple[int, str]], targets: dict[int, list[str]]) -> None:
    """Record every image word that points at a descriptor address."""
    if not targets:
        return
    for img in images:
        words = len(img) // 4
        for i in range(words):
            v = img.word(i)
            holders = targets.get(v)
            if holders is None:
                continue
            addr = img.vram_of(i * 4)
            if addr in targets:
                continue  # the descriptor's own words
            holders.append(f"{img.name}:{name_for(syms, addr)}")


def sweep(mk, disc: Disc, start: int, stop: int, cap_sectors: int,
          skip: set[int], progress: bool = True) -> list[Descriptor]:
    """Find record streams on the disc itself, descriptor or no descriptor.

    The engine builds most of its descriptors at runtime (`base + offset` into
    a container), so a scan of the executable only sees the boot and UI art.
    This finds the rest by trying every sector as a stream start and keeping
    the ones that unpack cleanly.
    """
    out: list[Descriptor] = []
    tried = 0
    for lba in disc.scan_heads(start, stop):
        if lba in skip:
            continue
        tried += 1
        if progress and tried % 2000 == 0:
            print(f"  ... {lba} / {stop}, {len(out)} found", file=sys.stderr)
        head = disc.sector(lba)
        if head is None:
            continue
        try:
            recs = parse_stream(head, strict=True, payload_limit=None)
        except BadStream:
            continue
        if not recs:
            continue
        try:
            end = resolve_payloads(mk, disc, lba, recs, cap_sectors)
        except BadStream:
            continue
        if end <= 0:
            continue
        nsec = (end + SECTOR_DATA - 1) // SECTOR_DATA
        if nsec > cap_sectors:
            continue
        blob = disc.read(lba, nsec)
        if blob is None:
            continue
        # re-validate against the real extent now that it is known
        try:
            recs = parse_stream(blob, strict=True)
            resolve_payloads(mk, disc, lba, recs, cap_sectors)
        except BadStream:
            continue
        d = Descriptor(lba=lba, nsec=nsec, dest=0, source="sweep")
        d.records = recs
        d.payload_bytes = end
        out.append(d)
    return out


# --------------------------------------------------------------------------
# reporting
# --------------------------------------------------------------------------


def px_width(words: int, depth: int) -> int:
    return words * (16 // depth)


def describe_record(rec: Record, depth: int) -> str:
    kind = TYPES[rec.type][0]
    head = f"  +{rec.offset:04x}  {kind:<13}"
    if rec.type in TEX_TYPES:
        x, y, w, h = rec.rect
        h = rec.rows or h
        body = (
            f"vram ({x:4d},{y:3d}) {w:3d}x{h:<3d} words"
            f"  = {px_width(w, depth):4d}x{h} px @{depth}bpp"
            f"   data +{rec.data_off:#x}"
        )
    elif rec.type in CLUT_TYPES:
        cx, cy, _cw, _ch = rec.clut_rect()
        body = (
            f"vram ({cx:4d},{cy:3d}) {rec.clut_entries:2d}x16 colors"
            f"  param {rec.clut_param:#06x} flags {rec.clut_flags:#x}"
            f"   data +{rec.data_off:#x}"
        )
    elif rec.type == 3:
        body = f"-> RAM +{rec.ram_off:#x} (lz)   data +{rec.data_off:#x}"
    elif rec.type == 4:
        body = (
            f"-> RAM {rec.ram_off:#010x} len {rec.ram_len:#x}"
            f"   data +{rec.data_off:#x}"
        )
    else:
        body = f"{rec.reloc_count} pointer pairs   data +{rec.data_off:#x}"
    if rec.size != 16:
        body += f"   [size {rec.size}]"
    for w_ in rec.warn:
        body += f"   ** {w_}"
    return head + body


def blob_tag(recs: list[Record]) -> str:
    kinds = {r.type for r in recs}
    parts = []
    if kinds & set(TEX_TYPES):
        parts.append("tex")
    if kinds & set(CLUT_TYPES):
        parts.append("clut")
    if kinds & set(RAM_TYPES):
        parts.append("payload")
    if 9 in kinds:
        parts.append("reloc")
    return "+".join(parts) or "empty"


def report(descs: list[Descriptor], depth: int, out=sys.stdout) -> None:
    for d in descs:
        dest = "buffer" if d.dest == 0 else f"{d.dest | K0BASE:#010x}"
        print(
            f"lba {d.lba:#07x} ({d.lba:6d})  nsec {d.nsec:3d}  "
            f"{d.nsec * SECTOR_DATA:7d} bytes  dest {dest}  "
            f"[{blob_tag(d.records)}]"
            + (f"  payload {d.payload_bytes} bytes" if d.payload_bytes else "")
            + ("  (sweep)" if d.source == "sweep" else ""),
            file=out,
        )
        if d.sites:
            print(f"    descriptor at {', '.join(sorted(set(d.sites)))}", file=out)
        if d.refs:
            print(f"    referenced by {', '.join(sorted(set(d.refs)))}", file=out)
        for rec in d.records:
            print(describe_record(rec, depth), file=out)
        print(file=out)


def summary(descs: list[Descriptor], out=sys.stdout) -> None:
    tex = sum(1 for d in descs for r in d.records if r.type in TEX_TYPES)
    clut = sum(1 for d in descs for r in d.records if r.type in CLUT_TYPES)
    ram = sum(1 for d in descs for r in d.records if r.type in RAM_TYPES)
    reloc = sum(1 for d in descs for r in d.records if r.type == 9)
    tags: dict[str, int] = {}
    for d in descs:
        t = blob_tag(d.records)
        tags[t] = tags.get(t, 0) + 1
    shape = ", ".join(f"{v} {k}" for k, v in sorted(tags.items(), key=lambda kv: -kv[1]))
    print(
        f"{len(descs)} blobs ({shape}): {tex} textures, {clut} palettes, "
        f"{ram} RAM payloads, {reloc} relocation records",
        file=out,
    )


def by_vram(descs: list[Descriptor], depth: int, out=sys.stdout) -> None:
    rows = []
    for d in descs:
        for r in d.records:
            if r.type in TEX_TYPES:
                x, y, w, h = r.rect
                rows.append((x, y, w, r.rows or h, d, r))
    rows.sort(key=lambda r: (r[0], r[1], r[2], r[3], r[4].lba, r[5].offset))
    print("textures by VRAM position:", file=out)
    for x, y, w, h, d, r in rows:
        h = r.rows or h
        print(
            f"  ({x:4d},{y:3d}) {px_width(w, depth):4d}x{h:<3d} px"
            f"  {r.kind:<8} lba {d.lba:#07x} +{r.offset:04x}",
            file=out,
        )


def to_json(descs: list[Descriptor]) -> list[dict]:
    out = []
    for d in descs:
        recs = []
        for r in d.records:
            e: dict = {
                "offset": r.offset,
                "type": r.type,
                "kind": r.kind,
                "arm": TYPES[r.type][1],
                "size": r.size,
                "data_offset": r.data_off,
            }
            if r.packed is not None:
                e["packed"] = r.packed
                e["unpacked"] = r.unpacked
            if r.type in TEX_TYPES:
                x, y, w, h = r.rect
                e["vram"] = {"x": x, "y": y, "w": w, "h": h}
                if r.rows is not None and r.rows != h:
                    e["vram"]["rows_supplied"] = r.rows
            elif r.type in CLUT_TYPES:
                cx, cy, cw, ch = r.clut_rect()
                e["clut"] = {
                    "param": r.clut_param,
                    "palettes": r.clut_entries,
                    "flags": r.clut_flags,
                    "vram": {"x": cx, "y": cy, "w": cw, "h": ch},
                }
            elif r.type == 3:
                e["ram_offset"] = r.ram_off
            elif r.type == 4:
                e["ram_dest"] = r.ram_off
                e["ram_len"] = r.ram_len
            else:
                e["pairs"] = r.reloc_count
            if r.warn:
                e["warnings"] = list(r.warn)
            recs.append(e)
        out.append(
            {
                "lba": d.lba,
                "nsec": d.nsec,
                "dest": d.dest,
                "bytes": d.nsec * SECTOR_DATA,
                "descriptor_sites": sorted(set(d.sites)),
                "referenced_by": sorted(set(d.refs)),
                "note": d.note,
                "source": d.source,
                "payload_bytes": d.payload_bytes,
                "tag": blob_tag(d.records),
                "records": recs,
            }
        )
    return out


# --------------------------------------------------------------------------
# decoding to PNG
# --------------------------------------------------------------------------


def clut_colors(mk, blob: bytes, rec: Record) -> list:
    n = rec.clut_entries * 16
    raw = list(struct.unpack_from(f"<{n}H", blob, rec.data_off))
    if rec.type == 7:
        # case 6: q[i] |= 0x8000 for i in 1 .. (entries << 4) - 1
        for i in range(1, n):
            raw[i] |= 0x8000
    elif rec.type == 8:
        # case 7: per group of 16, skip index 0 and set the other 15
        for g in range(rec.clut_entries):
            for i in range(1, 16):
                raw[g * 16 + i] |= 0x8000
    return [mk.raw_clut_to_rgba(v) for v in raw]


def texture_bytes(mk, blob: bytes, rec: Record) -> bytes:
    x, y, w, h = rec.rect
    want = w * (rec.rows or h) * 2
    if rec.type == 1:
        data = mk.decompress(io.BytesIO(blob[rec.data_off :]))
    else:
        data = blob[rec.data_off : rec.data_off + want]
    if len(data) < want:
        if rec.type == 5:
            # the overrun the game itself has; pad so the image still renders
            data = data + bytes(want - len(data))
        else:
            raise BadStream(
                f"record at {rec.offset:#x}: got {len(data)} bytes, rect wants {want}"
            )
    return data[:want]


def palette_score(colors: list, hist: dict[int, int]) -> float:
    """How much contrast an image actually gets out of a palette.

    A CLUT record holds up to 16 palettes and the record does not say which
    one a texture is drawn with -- the game picks that at draw time, from the
    primitive's CLUT id -- so any choice here is a guess.  Weighting each
    entry by how many pixels use it and taking the spread picks a legible
    variant instead of the flat wash that palette 0 often is, which is what
    this is for: triage.  Use --palette or --all-cluts for the real thing.

    Saturation is deliberately not in the score.  Favouring chroma picks the
    lurid end of a palette set (measured on the town atlases at lba 0x167e),
    and contrast alone reads better.
    """
    total = 0
    sums = [0.0, 0.0, 0.0]
    sqs = [0.0, 0.0, 0.0]
    for idx, count in hist.items():
        if idx >= len(colors):
            continue
        r, g, b, a = colors[idx]
        if a == 0:
            continue
        total += count
        for c, v in enumerate((r, g, b)):
            sums[c] += v * count
            sqs[c] += v * v * count
    if total == 0:
        return -1.0
    return sum(sqs[c] / total - (sums[c] / total) ** 2 for c in range(3))


def index_hist(raw: bytes, depth: int) -> dict[int, int]:
    hist: dict[int, int] = {}
    if depth == 8:
        for b in raw:
            hist[b] = hist.get(b, 0) + 1
    else:
        for b in raw:
            lo, hi = b & 0xF, b >> 4
            hist[lo] = hist.get(lo, 0) + 1
            hist[hi] = hist.get(hi, 0) + 1
    return hist


def palette_choices(mk, blob: bytes, cluts: list[Record], depth: int):
    """(clut record, group index, 256-entry colour list) for every candidate."""
    out = []
    for cr in cluts:
        try:
            colors = clut_colors(mk, blob, cr)
        except (struct.error, BadStream) as e:
            print(f"  clut at +{cr.offset:#x}: {e}", file=sys.stderr)
            continue
        if depth == 4:
            for g in range(max(1, cr.clut_entries)):
                sl = colors[g * 16 : (g + 1) * 16]
                if sl:
                    out.append((cr, g, sl))
        else:
            out.append((cr, 0, colors[:256]))
    return out


def write_pngs(mk, descs: list[Descriptor], disc: Disc, outdir: pathlib.Path,
               depth: int, all_cluts: bool, palette: Optional[int] = None) -> None:
    outdir.mkdir(parents=True, exist_ok=True)
    made = 0
    for d in descs:
        texs = [r for r in d.records if r.type in TEX_TYPES]
        cluts = [r for r in d.records if r.type in CLUT_TYPES]
        if not texs or not cluts:
            continue
        blob = disc.read(d.lba, d.nsec)
        if blob is None:
            continue
        cands = palette_choices(mk, blob, cluts, depth)
        if not cands:
            continue
        for tr in texs:
            try:
                raw = texture_bytes(mk, blob, tr)
            except (BadStream, AssertionError, IndexError) as e:
                print(f"  lba {d.lba:#x} +{tr.offset:#x}: {e}", file=sys.stderr)
                continue
            if all_cluts:
                chosen = cands
            elif palette is not None:
                chosen = [c for c in cands if c[1] == palette] or cands[:1]
            else:
                hist = index_hist(raw, depth)
                chosen = [max(cands, key=lambda c: palette_score(c[2], hist))]
            x, y, w, h = tr.rect
            rect = mk.Rect(x, y, w, tr.rows or h)
            for cr, group, colors in chosen:
                # pad so an index past the palette shows up instead of crashing
                pal = list(colors) + [(255, 0, 255, 255)] * 256
                try:
                    img = mk.draw_image(rect, pal, raw, depth)
                except Exception as e:  # noqa: BLE001 - keep going over a batch
                    print(f"  lba {d.lba:#x} +{tr.offset:#x}: {e}", file=sys.stderr)
                    continue
                stem = f"lba{d.lba:06x}_{tr.offset:04x}"
                if all_cluts or group:
                    stem += f"_c{cr.offset:04x}p{group:02d}"
                img.save(outdir / f"{stem}.png")
                made += 1
    print(f"wrote {made} png(s) to {outdir}", file=sys.stderr)


# --------------------------------------------------------------------------
# main
# --------------------------------------------------------------------------


def parse_image_spec(s: str) -> tuple[str, str, int, int, int]:
    parts = s.split(":")
    if len(parts) != 4:
        raise argparse.ArgumentTypeError(
            "expected FILE:FILEOFF:VRAM:LEN, e.g. data/town/town.bin:0xb000:0x80088760:0x46fc0"
        )
    path, off, vram, length = parts
    return (
        pathlib.Path(path).name,
        path,
        int(off, 0),
        int(vram, 0),
        int(length, 0),
    )


def main() -> None:
    ap = argparse.ArgumentParser(
        prog="scan-img.py",
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    ap.add_argument("--disc", default=str(ROOT / "az.bin"), help="MODE2/2352 image")
    ap.add_argument(
        "--image",
        action="append",
        type=parse_image_spec,
        metavar="FILE:FILEOFF:VRAM:LEN",
        help="scan this image instead of the defaults (repeatable)",
    )
    ap.add_argument("--lba", type=lambda s: int(s, 0), help="decode one blob, no scan")
    ap.add_argument(
        "--sweep",
        action="store_true",
        help="also try every disc sector as a stream start; finds the blobs "
        "whose descriptors the engine builds at runtime (~10s for the disc)",
    )
    ap.add_argument("--sweep-from", type=lambda s: int(s, 0), default=0)
    ap.add_argument("--sweep-to", type=lambda s: int(s, 0), default=None)
    ap.add_argument("--no-exe-scan", action="store_true",
                    help="skip the executable scan; use with --sweep")
    ap.add_argument("--cap", type=lambda s: int(s, 0), default=1024,
                    metavar="SECTORS", help="largest blob the sweep will accept")
    ap.add_argument("--nsec", type=lambda s: int(s, 0), default=1, help="with --lba")
    ap.add_argument(
        "--loose",
        action="store_true",
        help="keep blobs whose stream only partly validates (noisy)",
    )
    ap.add_argument("--depth", type=int, choices=(4, 8), default=4,
                    help="colour depth assumed when reporting/rendering (default 4)")
    ap.add_argument("--json", metavar="FILE", help="write the manifest as JSON")
    ap.add_argument("--extract", metavar="DIR", help="write each raw blob")
    ap.add_argument("--png", metavar="DIR", help="decode textures to PNG")
    ap.add_argument("--all-cluts", action="store_true",
                    help="with --png, render each texture against every palette in its blob")
    ap.add_argument("--palette", type=lambda s: int(s, 0), default=None,
                    metavar="N",
                    help="with --png, use palette N of each CLUT record "
                    "(default: pick the one the texture reads most clearly "
                    "with; the real choice is made at draw time)")
    ap.add_argument("--by-vram", action="store_true", help="also list textures by VRAM position")
    ap.add_argument("--quiet", action="store_true", help="summary only")
    args = ap.parse_args()

    disc = Disc(pathlib.Path(args.disc))

    descs: list[Descriptor] = []

    if args.lba is not None:
        blob = disc.read(args.lba, args.nsec)
        if blob is None:
            raise SystemExit(f"cannot read {args.nsec} sector(s) at lba {args.lba:#x}")
        d = Descriptor(lba=args.lba, nsec=args.nsec, dest=0)
        try:
            d.records = parse_stream(blob, strict=not args.loose)
        except BadStream as e:
            raise SystemExit(f"lba {args.lba:#x}: not a record stream: {e}")
        try:
            d.payload_bytes = resolve_payloads(
                _load_make_img(), disc, args.lba, d.records, max(args.nsec, 1)
            )
        except BadStream as e:
            d.note = str(e)
        descs = [d]
    else:
        mk = _load_make_img()
        found: dict[tuple[int, int], Descriptor] = {}
        rejected = 0
        images: list[Image] = []
        syms: list[tuple[int, str]] = []

        # the executable scan
        if not args.no_exe_scan:
            images = load_images(args.image or DEFAULT_IMAGES)
            if not images:
                raise SystemExit("no images to scan")
            syms = load_symbols()
        for img in images:
            for off, lba, nsec, dest in descriptor_candidates(img, disc):
                key = (lba, nsec)
                site = f"{img.name}:{name_for(syms, img.vram_of(off))}"
                if key in found:
                    found[key].sites.append(site)
                    continue
                blob = disc.read(lba, nsec)
                if blob is None:
                    continue
                try:
                    recs = parse_stream(blob, strict=not args.loose)
                except BadStream:
                    rejected += 1
                    continue
                d = Descriptor(lba=lba, nsec=nsec, dest=dest)
                d.sites.append(site)
                d.records = recs
                # measure the compressed payloads; a descriptor is authoritative
                # about its own blob, so a failure here is a note, not a veto
                try:
                    d.payload_bytes = resolve_payloads(mk, disc, lba, recs, max(nsec, 1))
                except BadStream as e:
                    d.note = str(e)
                found[key] = d

        if args.sweep:
            stop = args.sweep_to if args.sweep_to is not None else disc.sectors
            skip = {lba for lba, _ in found}
            print(
                f"sweeping sectors {args.sweep_from}..{stop} "
                f"(cap {args.cap} sectors/blob)",
                file=sys.stderr,
            )
            for d in sweep(mk, disc, args.sweep_from, stop, args.cap, skip):
                found.setdefault(d.key, d)

        descs = sorted(found.values(), key=lambda d: d.key)

        # name the places that point at each accepted descriptor
        targets: dict[int, list[str]] = {}
        for img in images:
            for off, lba, nsec, _dest in descriptor_candidates(img, disc):
                d = found.get((lba, nsec))
                if d is not None:
                    targets[img.vram_of(off)] = d.refs
        crossref(images, syms, targets)

        n_desc = sum(1 for d in descs if d.source == "descriptor")
        n_sweep = len(descs) - n_desc
        print(
            f"scanned {len(images)} image(s): {n_desc} blobs from descriptors "
            f"({rejected} descriptor-shaped pairs rejected)"
            + (f", {n_sweep} more from the disc sweep" if args.sweep else ""),
            file=sys.stderr,
        )

    if not args.quiet:
        report(descs, args.depth)
        if args.by_vram:
            by_vram(descs, args.depth)
    summary(descs)

    if args.json:
        with open(args.json, "w") as f:
            json.dump(to_json(descs), f, indent=2)
        print(f"wrote {args.json}", file=sys.stderr)

    if args.extract:
        outdir = pathlib.Path(args.extract)
        outdir.mkdir(parents=True, exist_ok=True)
        for d in descs:
            blob = disc.read(d.lba, d.nsec)
            if blob is None:
                continue
            (outdir / f"lba{d.lba:06x}_{d.nsec:03d}.bin").write_bytes(blob)
        print(f"wrote {len(descs)} blob(s) to {outdir}", file=sys.stderr)

    if args.png:
        write_pngs(
            _load_make_img(), descs, disc, pathlib.Path(args.png),
            args.depth, args.all_cluts, args.palette,
        )


if __name__ == "__main__":
    main()
