"""
Convert the `.shiftjis` directive in an assembly file to the byte pattern it
corresponds to. Necessary since GNU AS macros aren't expressive enough and
doesn't support custom directives.
"""

import argparse
import re
import sys
from typing import TextIO

SHIFTJIS_DIRECTIVE_PATTERN = re.compile(r"^(\s*)\.shiftjis \"(.*?)\"(\s*)$")


def convert_shiftjis(f: TextIO) -> None:
    for line in f:
        match = SHIFTJIS_DIRECTIVE_PATTERN.match(line)
        if match is None:
            print(line, end="")
            continue
        encoded = match.group(2).encode("shift-jis")
        literals = ", ".join("0x{:02x}".format(byte) for byte in encoded)
        print(match.group(1), end="")
        print(f".byte {literals}", end="")
        print(match.group(3), end="")


def setup_parser(prog: str) -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(prog)
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
    main(setup_parser("convert_shiftjis"))
