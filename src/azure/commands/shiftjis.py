import argparse
import re
import sys
from typing import TextIO

# The string body may carry the escapes `splat/ext/shiftjis.py` writes, so it
# is matched as "either a plain character or a backslash pair" rather than
# with a non-greedy `.*?`, which would stop at the first escaped quote.
#
# `.shiftjisz` is the same directive plus the layout every string table in this
# game uses: a NUL terminator and then padding to the next 4-byte boundary, so
# the run is `4 - (len % 4)` zero bytes -- 1, 2, 3 or 4, never none. Measured
# on the item/spell pool at file offset 0x2810, which is uniformly laid out
# this way. It makes an entry self-contained: a label, a string, and the bytes
# that carry the next label to its boundary, with no state kept between
# entries and nothing to close at the end of the listing.
SHIFTJIS_DIRECTIVE_PATTERN = re.compile(
    r"^(\s*)\.shiftjis(z?) \"((?:[^\"\\]|\\.)*)\"(\s*)$"
)
ESCAPE_PATTERN = re.compile(r"\\(\\|\"|x[0-9a-fA-F]{2})")


def unescape(body: str) -> str:
    def sub(match: re.Match) -> str:
        esc = match.group(1)
        if esc.startswith("x"):
            return chr(int(esc[1:], 16))
        return esc

    return ESCAPE_PATTERN.sub(sub, body)


def convert_shiftjis(f: TextIO) -> None:
    for line in f:
        match = SHIFTJIS_DIRECTIVE_PATTERN.match(line)
        if match is None:
            print(line, end="")
            continue
        indent, terminate, body, trailing = match.groups()
        encoded = unescape(body).encode("shift-jis")
        if terminate:
            encoded += b"\x00"
        literals = ", ".join("0x{:02x}".format(byte) for byte in encoded)
        print(indent, end="")
        print(f".byte {literals}", end="")
        print(trailing, end="")
        if terminate:
            # `.balign` pads a data section with zeros, and emits nothing when
            # the terminator already landed on the boundary.
            if not trailing.endswith("\n"):
                print()
            print(f"{indent}.balign 4, 0")


def setup_parser(prog: str) -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog,
        description="""
            Convert the `.shiftjis` directive in an assembly file to the byte
            pattern it corresponds to. Necessary since GNU AS macros aren't
            expressive enough and doesn't support custom directives.
        """,
    )
    parser.add_argument("input", nargs="?", default="-")
    parser.set_defaults(main=main)
    return parser


def main(args: argparse.Namespace) -> None:
    if args.input != "-":
        with open(args.input, "r") as f:
            convert_shiftjis(f)
    else:
        convert_shiftjis(sys.stdin)


if __name__ == "__main__":
    parser = setup_parser("convert_shiftjis")
    main(parser.parse_args())
