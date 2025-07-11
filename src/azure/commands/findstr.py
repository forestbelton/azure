import argparse
import dataclasses
import os
import re
from typing import BinaryIO, Optional

MIN_LENGTH = 3


@dataclasses.dataclass
class DetectedString:
    offset: int
    s: str
    raw: bytes


SB_CHARS = {0x3, 0x8, 0xA, 0x11}
MB_CHARS = {0x81, 0x82, 0xFE}


def find_shiftjis(f: BinaryIO) -> list[DetectedString]:
    f.seek(0, os.SEEK_END)
    file_size = f.tell()
    i = 0
    strs: list[DetectedString] = []
    while i < file_size:
        s = find_shiftjis_at(f, i)
        if s is not None:
            strs.append(s)
            i += len(s.raw)
        else:
            i += 1
    return strs


# From Wikipedia:
#
# If the first byte is odd, the second byte must be in the range 0x40 to 0x9E
# (but cannot be 0x7F); if the first byte is even, the second byte must in the
# range 0x9F to 0xFC.
def valid_second_byte(b1: int, b2: int) -> bool:
    if b1 == 0xFE:
        return b2 in {0x00, 0x01}
    if b1 % 2 == 0:
        return b2 >= 0x40 and b2 <= 0xFC
    else:
        return b2 >= 0x40 and b2 <= 0x9E and b2 != 0x7F


def escape_surrogates(s: str) -> str:
    return re.sub(r"[\udc80-\udcff]", lambda m: f"\\u{ord(m.group()):04x}", s)


def find_shiftjis_at(f: BinaryIO, offset: int) -> Optional[DetectedString]:
    f.seek(offset, os.SEEK_SET)
    first_byte = f.read(1)
    if len(first_byte) == 0:
        return None
    bs: list[int] = []
    if first_byte[0] in MB_CHARS:
        bs.extend(first_byte)
        second_byte = f.read(1)
        if len(second_byte) == 0:
            return None
        if not valid_second_byte(first_byte[0], second_byte[0]):
            return None
        bs.extend(second_byte)
    else:
        return None
    b = f.read(1)
    while len(b) > 0:
        if b[0] == 0:
            break
        if b[0] in SB_CHARS:
            bs.extend(b)
        elif b[0] in MB_CHARS:
            bs.extend(b)
            b2 = f.read(1)
            assert len(b2) > 0
            if not valid_second_byte(b[0], b2[0]):
                break
            bs.extend(b2)
        else:
            break
        b = f.read(1)
    if len(bs) < MIN_LENGTH * 2:
        return None
    raw = bytes(bs)
    return DetectedString(
        offset=offset, s=raw.decode("shift-jis", errors="surrogateescape"), raw=raw
    )


def setup_parser(prog: str) -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog=prog,
        description="""
            Attempt to identify AD Shift-JIS strings inside of a binary
        """,
    )
    parser.add_argument("file")
    parser.set_defaults(main=main)
    return parser


def main(args: argparse.Namespace) -> None:
    with open(args.file, "rb") as f:
        strs = find_shiftjis(f)
    for loc in strs:
        print(
            f"""{{
    "offset": {hex(loc.offset)},
    "length": {len(loc.raw)},
    "str": "{escape_surrogates(loc.s)}"
}}"""
        )


if __name__ == "__main__":
    parser = setup_parser("find_shiftjis_strings")
    main(parser.parse_args())
