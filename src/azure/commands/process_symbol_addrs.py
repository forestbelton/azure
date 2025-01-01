"""
Perform various operations over a splat symbol address file.
"""

import argparse
import re
from typing import Optional

import yaml

from azure.commands import extract_symbol_info

SYMBOL_ADDR_PATTERN = re.compile(r"^(\w+)\s*=\s*0x([a-fA-F0-9]+);.*$")


def get_addr(raw_addr: str) -> int:
    addr = int(raw_addr, base=16) - 0x8002D000 + 0x800
    assert addr >= 0
    return addr


def convert_line(line: str, info: extract_symbol_info.SymInfoMap = {}) -> Optional[str]:
    m = SYMBOL_ADDR_PATTERN.match(line)
    if m is None:
        return
    name = m.group(1)
    filename = ""
    if name in info:
        filename = f", psyq/{info[name].library.lower()}/{info[name].module}"
    addr = get_addr(m.group(2))
    return f"      - [{hex(addr)}, asm{filename}] # {name}"


def convert_file(filename: str, info: extract_symbol_info.SymInfoMap) -> None:
    with open(filename, "r") as f:
        for line in f:
            yaml_line = convert_line(line, info)
            if yaml_line is not None:
                print(yaml_line)


def sort_file(filename: str) -> None:
    with open(filename, "r") as f:
        lines = [line for line in f if SYMBOL_ADDR_PATTERN.match(line)]

    def sort_key(line: str) -> int:
        m = SYMBOL_ADDR_PATTERN.match(line)
        return get_addr(m.group(2))

    lines.sort(key=sort_key)
    for line in lines:
        print(line.strip())


def validate_file(addrs_filename: str) -> None:
    with open(addrs_filename, "r") as f:
        lines = [line for line in f if SYMBOL_ADDR_PATTERN.match(line)]


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--lib_dir", default=None)
    parser.add_argument(
        "--action", default="yaml", choices=["yaml", "sort", "validate"]
    )
    parser.add_argument("file")
    args = parser.parse_args()
    info = {}
    if args.lib_dir is not None:
        info = extract_symbol_info.extract_dir(args.lib_dir)
    if args.action == "yaml":
        convert_file(args.file, info)
    elif args.action == "sort":
        sort_file(args.file)
    elif args.action == "validate":
        validate_file(args.file)


if __name__ == "__main__":
    main()
