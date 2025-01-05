import argparse
import dataclasses
import os
import struct
from typing import BinaryIO, Optional

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


def extract_bin(f: BinaryIO) -> None:
    f.seek(0, os.SEEK_END)
    file_size = f.tell()
    sector_size = SECTOR_SIZE
    if file_size == 0 or file_size % sector_size != 0:
        raise ExtractException(
            f"{file_size=} invalid (expected a multiple of {sector_size=})"
        )
    num_sectors = int(file_size / sector_size)
    print(f"{num_sectors=}")
    f.seek(0, os.SEEK_SET)
    for sector_index in range(num_sectors):
        raw_sector = f.read(sector_size)
        sector_len = len(raw_sector)
        if sector_len != sector_size:
            raise ExtractException(f"{sector_len=} invalid (expected {sector_size=})")
        sector = parse_sector_bytes(sector_index, raw_sector)
        print(
            f"sector {sector.minutes}:{sector.seconds}:{sector.frame} mode={sector.mode} form={((sector.subheader.submode>>5)&1) + 1} {sector.subheader}"
        )
    offset = f.tell()
    if offset != file_size:
        raise ExtractException(
            f"unexpected extra {file_size-offset} bytes at {offset=}"
        )


def setup_parser(prog: str) -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(prog=prog)
    parser.add_argument("--extract", action="store_true", required=True)
    parser.add_argument("file")
    parser.set_defaults(main=main)
    return parser


def main(args: argparse.Namespace) -> None:
    with open(args.file, "rb") as f:
        extract_bin(f)


if __name__ == "__main__":
    parser = setup_parser("bincue")
    main(parser.parse_args())
