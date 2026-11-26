import argparse
import enum
import pathlib
import sys
from typing import BinaryIO, Optional

import hashlib

from pydantic import dataclasses
import yaml

from azure.commands import bincue

# A MODE2/FORM1 sector's user data. FORM2 (XA-ADPCM audio) sectors carry 2324
# instead, so this is the floor on what a sector contributes, not the rule.
FORM1_DATA_SIZE = 2048


class FileType(enum.StrEnum):
    """What a disc file holds.

    This is descriptive. Extraction does not branch on it: every file comes off
    the disc the same way -- read the entry's ``sectors`` sectors, keep each
    one's payload, and truncate to ``size`` if the entry gives one. The type
    records what those sectors *are*, and selects files for ``--only``.

    ``data``         Plain MODE2/FORM1. Every sector carries 2048 bytes.
    ``audio-video``  A CD-XA stream: MDEC video in FORM1 sectors interleaved
                     7:1 with XA-ADPCM audio in FORM2 sectors.
    ``audio``        A CD-XA stream with no video sectors -- XA-ADPCM audio
                     padded out with blank FORM1 sectors to pace the drive.
    """

    DATA = "data"
    AUDIO_VIDEO = "audio-video"
    AUDIO = "audio"


@dataclasses.dataclass
class SpecFile:
    name: str
    shasum: str
    # Where the file sits on the disc: first LBA, and the extent in sectors.
    # The extent is what the spec records rather than a byte length, because a
    # byte length cannot be turned back into a sector count -- FORM1 sectors
    # contribute 2048 bytes and FORM2 sectors 2324, so for an interleaved
    # stream the length depends on the mix.
    sector: int
    sectors: int
    type: FileType = FileType.DATA
    # Optional: truncate the extracted payload to this many bytes. Only a file
    # whose real content ends mid-sector needs it (on this disc, just
    # system.cnf). It cannot recover the sector-aligned filler the mastering
    # left inside the .BIN containers, because that filler also falls in the
    # middle of them, not only at the tail.
    size: Optional[int] = None

    def __post_init__(self) -> None:
        if self.sectors < 1:
            raise ValueError(f"{self.name}: 'sectors' must be at least 1")
        if self.size is None:
            return
        if self.size < 0:
            raise ValueError(f"{self.name}: 'size' must not be negative")
        # Every sector yields at least FORM1_DATA_SIZE bytes, so this bound
        # holds whatever the interleave is, without reading the disc.
        limit = self.sectors * FORM1_DATA_SIZE
        if self.size > limit:
            raise ValueError(
                f"{self.name}: 'size' {self.size} exceeds {self.sectors} "
                f"sectors x {FORM1_DATA_SIZE} = {limit} bytes"
            )


@dataclasses.dataclass
class Spec:
    infile: str
    shasum: str
    outdir: str
    files: list[SpecFile]


Path = pathlib.Path | str


def extract_sectors(
    f: BinaryIO, start_sector: int, num_sectors: int, size: Optional[int] = None
) -> bytes:
    """Read `num_sectors` sectors from `start_sector`, keeping every payload byte.

    Each sector contributes whatever its own form says -- 2048 bytes for FORM1,
    2324 for FORM2 -- so the result is as long as the interleave makes it. With
    `size`, the result is truncated to that many bytes.
    """
    data = bytearray()
    for offset in range(num_sectors):
        data += bincue.extract_sector(f, start_sector + offset).data
    if size is not None:
        if size > len(data):
            raise ValueError(
                f"size {size} exceeds the {len(data)} bytes in "
                f"{num_sectors} sectors at {start_sector}"
            )
        del data[size:]
    return bytes(data)


def extract_spec_file(f: BinaryIO, spec_file: SpecFile) -> bytes:
    return extract_sectors(
        f, spec_file.sector, spec_file.sectors, spec_file.size
    )


def get_file_shasum(filename: Path) -> str:
    hash = hashlib.sha1()
    with open(filename, "rb") as f:
        for chunk in iter(lambda: f.read(128 * hash.block_size), b""):
            hash.update(chunk)
    return hash.hexdigest()


def setup_parser(name: str) -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser()
    parser.add_argument("spec")
    parser.add_argument(
        "--only",
        action="append",
        metavar="TYPE",
        choices=[t.value for t in FileType],
        help="extract only files of this type (repeatable)",
    )
    parser.set_defaults(main=main)
    return parser


def main(args: argparse.Namespace):
    with open(args.spec, "r") as f:
        raw_spec = yaml.full_load(f)
    spec = Spec(**raw_spec)
    infile_shasum = get_file_shasum(spec.infile)
    if spec.shasum != infile_shasum:
        print(f"ERROR: input file does not have expected shasum", file=sys.stderr)
        print(f"actual:   {infile_shasum}", file=sys.stderr)
        print(f"expected: {spec.shasum}", file=sys.stderr)
        sys.exit(1)
    wanted = set(FileType(t) for t in args.only) if args.only else None
    outdir = pathlib.Path(spec.outdir)
    with open(spec.infile, "rb") as f:
        for spec_file in spec.files:
            if wanted is not None and spec_file.type not in wanted:
                continue
            spec_outfile = outdir / spec_file.name
            spec_outfile.parent.mkdir(parents=True, exist_ok=True)
            with open(spec_outfile, "wb") as outf:
                outf.write(extract_spec_file(f, spec_file))
            outfile_shasum = get_file_shasum(spec_outfile)
            if spec_file.shasum != outfile_shasum:
                print(
                    f"ERROR: extracted file {spec_file.name} does not have expected shasum",
                    file=sys.stderr,
                )
                print(f"actual:   {outfile_shasum}", file=sys.stderr)
                print(f"expected: {spec_file.shasum}", file=sys.stderr)
                sys.exit(1)


if __name__ == "__main__":
    parser = setup_parser("spec")
    main(parser.parse_args())
