import argparse
import dataclasses
import os
import struct
from typing import BinaryIO, Iterator, Optional

SECTOR_SIZE = 2352

SYNC_PATTERN = bytes(
    [0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00]
)


class ExtractException(Exception):
    pass


@dataclasses.dataclass
class SectorSubheader:
    file: int
    channel: int
    submode: int
    coding: int


@dataclasses.dataclass
class Sector:
    minutes: int
    seconds: int
    frame: int
    mode: int
    subheader: Optional[SectorSubheader]
    edc: Optional[int]
    ecc: Optional[bytes]
    data: bytes


def from_bcd(bcd: int) -> int:
    hi = bcd >> 0x4
    assert hi < 0xA
    lo = bcd & 0xF
    assert lo < 0xA
    return hi * 10 + lo


def parse_sector_bytes(sector_index: int, raw_sector: bytes) -> Sector:
    assert len(raw_sector) == SECTOR_SIZE
    sync_pattern, minutes, seconds, frame, mode = struct.unpack(
        "12sBBBB", raw_sector[:16]
    )
    if sync_pattern != SYNC_PATTERN:
        raise ExtractException(f"{sector_index=}: unexpected {sync_pattern=}")
    if mode != 2:
        raise ExtractException(f"{sector_index=}: unexpected sector {mode=}")
    subheader1 = struct.unpack("BBBB", raw_sector[16:20])
    subheader2 = struct.unpack("BBBB", raw_sector[20:24])
    if subheader1 != subheader2:
        raise ExtractException(f"{sector_index=}: unexpected cd-da sector (mode 2)")
    file, channel, submode, coding = subheader1
    # form 2
    if submode & (1 << 5):
        data = raw_sector[24 : 24 + 2324]
        (edc,) = struct.unpack("<I", raw_sector[24 + 2324 :])
        ecc = None
    # form 1
    else:
        data = raw_sector[24 : 24 + 2048]
        (edc,) = struct.unpack("<I", raw_sector[24 + 2048 : 24 + 2048 + 4])
        ecc = raw_sector[24 + 2048 + 4 :]
        assert len(ecc) == 276
    return Sector(
        minutes=from_bcd(minutes),
        seconds=from_bcd(seconds),
        frame=from_bcd(frame),
        mode=mode,
        subheader=SectorSubheader(
            file=file,
            channel=channel,
            submode=submode,
            coding=coding,
        ),
        edc=edc,
        ecc=ecc,
        data=data,
    )


def extract_sector(f: BinaryIO, sector_index: int) -> Sector:
    sector_start_offset = sector_index * SECTOR_SIZE
    f.seek(sector_start_offset, os.SEEK_SET)
    assert f.tell() == sector_start_offset
    raw_sector = f.read(SECTOR_SIZE)
    assert len(raw_sector) == SECTOR_SIZE
    return parse_sector_bytes(sector_index, raw_sector)


def bin_sectors(f: BinaryIO) -> Iterator[tuple[int, Sector]]:
    f.seek(0, os.SEEK_END)
    file_size = f.tell()
    sector_size = SECTOR_SIZE
    if file_size == 0 or file_size % sector_size != 0:
        raise ExtractException(
            f"{file_size=} invalid (expected a multiple of {sector_size=})"
        )
    num_sectors = int(file_size / sector_size)
    f.seek(0, os.SEEK_SET)
    for sector_index in range(num_sectors):
        raw_sector = f.read(sector_size)
        sector_len = len(raw_sector)
        if sector_len != sector_size:
            raise ExtractException(f"{sector_len=} invalid (expected {sector_size=})")
        yield sector_index, parse_sector_bytes(sector_index, raw_sector)
    offset = f.tell()
    if offset != file_size:
        raise ExtractException(
            f"unexpected extra {file_size-offset} bytes at {offset=}"
        )


def extract_sectors(args: argparse.Namespace):
    lba = int(args.lba, 16)
    num = int(args.len, 16)
    found = False
    count = 0
    with open(args.infile, "rb") as inf, open(args.outfile, "wb") as outf:
        for sector_index, sector in bin_sectors(inf):
            if sector_index == lba:
                found = True
            if found and count < num:
                outf.write(sector.data)
                count += 1
                if count == num:
                    return


def find_sector(args: argparse.Namespace):
    prefix = bytes.fromhex(args.hex)
    with open(args.binfile, "rb") as f:
        for sector_index, sector in bin_sectors(f):
            if sector.data.startswith(prefix):
                print("sector=" + hex(sector_index))
                print("offset=" + hex(SECTOR_SIZE * sector_index + 24))


def extract_bin(f: BinaryIO) -> None:
    for _, sector in bin_sectors(f):
        print(
            f"sector {sector.minutes}:{sector.seconds}:{sector.frame} mode={sector.mode} form={((sector.subheader.submode>>5)&1) + 1} {sector.subheader}"
        )


def setup_parser(prog: str) -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(prog=prog)
    subparsers = parser.add_subparsers(required=True)
    get_sector = subparsers.add_parser("sector", conflict_handler="resolve")
    get_sector.add_argument("--lba", required=True)
    get_sector.add_argument("--len", default="0x01", required=True)
    get_sector.add_argument("infile")
    get_sector.add_argument("outfile")
    get_sector.set_defaults(main=extract_sectors)
    find_sector_p = subparsers.add_parser("find", conflict_handler="resolve")
    find_sector_p.add_argument("--hex", required=True)
    find_sector_p.add_argument("binfile")
    find_sector_p.set_defaults(main=find_sector)
    parser.set_defaults(main=main)
    return parser


def main(args: argparse.Namespace) -> None:
    args.main(args)


if __name__ == "__main__":
    parser = setup_parser("bincue")
    main(parser.parse_args())
