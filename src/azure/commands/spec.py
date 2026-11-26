import argparse
import enum
import pathlib
import sys
from typing import BinaryIO, Optional

import hashlib

from pydantic import dataclasses
import yaml

from azure.commands import bincue


class FileType(enum.StrEnum):
    """What a disc file holds, which decides how it comes off the disc.

    ``data`` is a plain MODE2/FORM1 file -- every sector carries 2048 bytes of
    user data and the ISO9660 directory records a true byte length, so ``size``
    drives the extraction and the sector count follows from it.

    ``audio-video`` and ``audio`` are CD-XA streams.  Their sectors are
    interleaved: FORM1 sectors carry 2048 bytes, FORM2 (XA-ADPCM audio) sectors
    carry 2324, so a byte length cannot be turned back into a sector count.
    The on-disc extent in ``sectors`` drives the extraction instead, and every
    sector's payload is kept -- the audio cannot be separated out without
    demuxing, and the interleave is the file.
    """

    DATA = "data"
    AUDIO_VIDEO = "audio-video"
    AUDIO = "audio"


@dataclasses.dataclass
class SpecFile:
    name: str
    shasum: str
    sector: int
    type: FileType = FileType.DATA
    # Exactly one of these, picked by `type`: `size` for data, `sectors` for
    # the XA streams.  See FileType.
    size: Optional[int] = None
    sectors: Optional[int] = None

    def __post_init__(self) -> None:
        if self.type is FileType.DATA:
            if self.size is None:
                raise ValueError(f"{self.name}: type '{self.type}' needs 'size'")
            if self.sectors is not None:
                raise ValueError(
                    f"{self.name}: type '{self.type}' takes 'size', not 'sectors'"
                )
        else:
            if self.sectors is None:
                raise ValueError(f"{self.name}: type '{self.type}' needs 'sectors'")
            if self.size is not None:
                raise ValueError(
                    f"{self.name}: type '{self.type}' takes 'sectors', not 'size'"
                )


@dataclasses.dataclass
class Spec:
    infile: str
    shasum: str
    outdir: str
    files: list[SpecFile]


Path = pathlib.Path | str


def extract_file(f: BinaryIO, start_sector: int, file_size: int) -> bytes:
    """Read a plain data file: sectors until `file_size` bytes are in hand."""
    data = bytearray()
    sector_index = start_sector
    while len(data) < file_size:
        sector = bincue.extract_sector(f, sector_index)
        bytes_in_chunk = min(len(sector.data), file_size - len(data))
        data += sector.data[:bytes_in_chunk]
        sector_index += 1
    return bytes(data)


def extract_sectors(f: BinaryIO, start_sector: int, num_sectors: int) -> bytes:
    """Read an XA stream: every payload byte of `num_sectors` sectors.

    Each sector contributes whatever its own form says -- 2048 bytes for FORM1,
    2324 for FORM2 -- so the result is as long as the interleave makes it.
    """
    data = bytearray()
    for offset in range(num_sectors):
        data += bincue.extract_sector(f, start_sector + offset).data
    return bytes(data)


def extract_spec_file(f: BinaryIO, spec_file: SpecFile) -> bytes:
    if spec_file.type is FileType.DATA:
        assert spec_file.size is not None
        return extract_file(f, spec_file.sector, spec_file.size)
    assert spec_file.sectors is not None
    return extract_sectors(f, spec_file.sector, spec_file.sectors)


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
