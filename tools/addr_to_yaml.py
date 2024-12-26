import argparse
import re
from typing import Optional

SYMBOL_ADDR_PATTERN = re.compile(r"^(\w+) = 0x([a-fA-F0-9]+); .*$")


def convert_line(line: str) -> Optional[str]:
    m = SYMBOL_ADDR_PATTERN.match(line)
    if m is None:
        return
    name = m.group(1)
    addr = int(m.group(2), base=16) - 0x8002D000 + 0x800
    assert addr >= 0
    return f"      - [{hex(addr)}, asm] # {name}"


def convert_file(filename: str) -> None:
    with open(filename, "r") as f:
        for line in f:
            yaml_line = convert_line(line)
            if yaml_line is not None:
                print(yaml_line)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("file")
    args = parser.parse_args()
    convert_file(args.file)


if __name__ == "__main__":
    main()
