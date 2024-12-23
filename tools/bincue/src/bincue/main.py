import argparse
import os
import struct
from typing import BinaryIO

SECTOR_SIZE = 2352

SYNC_PATTERN = bytes(
    [00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00]
)


class ExtractException(Exception):
    pass


def from_bcd(bcd: int) -> int:
    hi = bcd >> 0x4
    assert hi < 0xA
    lo = bcd & 0xF
    assert lo < 0xA
    return hi * 10 + lo


def extract_sector(sector_index: int, raw_sector: bytes) -> None:
    #         0       12      Sync pattern (00 FF FF FF FF FF FF FF FF FF FF 00)
    # 12      4       Sector header (minutes, seconds, sectors, mode)
    # 16      8       Subheader (including Form 1/2 flags if used)
    # 24      2324    Data field
    # 2348    4       EDC (if Form 1 or Form 2)
    sync_pattern, minutes, seconds, frame, mode = struct.unpack(
        "12sBBBB", raw_sector[:16]
    )
    minutes = from_bcd(minutes)
    seconds = from_bcd(seconds)
    frame = from_bcd(frame)
    if sync_pattern != SYNC_PATTERN:
        raise ExtractException(f"{sector_index=}: unexpected {sync_pattern=}")
    if mode != 2:
        raise ExtractException(f"{sector_index=}: unexpected sector {mode=}")
    subheader1 = struct.unpack("BBBB", raw_sector[16:20])
    subheader2 = struct.unpack("BBBB", raw_sector[20:24])
    if subheader1 != subheader2:
        raise ExtractException(f"{sector_index=}: unexpected cd-da sector (mode 2)")
    file, channel, submode, coding = subheader1
    print(
        f"{sector_index=}: {minutes:02}:{seconds:02}:{frame:02} {mode=} {file=} {channel=} {submode=:02x} {coding=}"
    )


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
        extract_sector(sector_index, raw_sector)
    offset = f.tell()
    if offset != file_size:
        raise ExtractException(
            f"unexpected extra {file_size-offset} bytes at {offset=}"
        )


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--extract", action="store_true", required=True)
    parser.add_argument("file")
    args = parser.parse_args()
    with open(args.file, "rb") as f:
        extract_bin(f)


if __name__ == "__main__":
    main()
