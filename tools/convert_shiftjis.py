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


def main() -> None:
    parser = argparse.ArgumentParser("convert_shiftjis")
    parser.add_argument("input", nargs="?", default="-")
    args = parser.parse_args()
    if args.input != "-":
        with open(args.input, "r") as f:
            convert_shiftjis(f)
    else:
        convert_shiftjis(sys.stdin)


if __name__ == "__main__":
    main()
