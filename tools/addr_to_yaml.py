import argparse
import re

SYMBOL_ADDR_PATTERN = re.compile(r"^(\w+) = 0x([a-fA-F0-9]+); .*$")


def addr_to_yaml(filename: str) -> None:
    with open(filename, "r") as f:
        for line in f:
            m = SYMBOL_ADDR_PATTERN.match(line)
            if m is None:
                continue
            name = m.group(1)
            addr = int(m.group(2), base=16) - 0x8002D000 + 0x800
            assert addr >= 0
            print(f"      - [{hex(addr)}, asm] # {name}")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("file")
    args = parser.parse_args()
    addr_to_yaml(args.file)


if __name__ == "__main__":
    main()
