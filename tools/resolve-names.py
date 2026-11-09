import argparse
import re
from typing import TextIO

Symbols = dict[str, str]

SYMBOL_PATTERN = re.compile(r"\s*([a-zA-Z_][a-zA-Z0-9_]*) = 0x([a-fA-F0-9]{8});\s*$")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--symfile", required=True)
    parser.add_argument("--infile", required=True)
    args = parser.parse_args()
    with open(args.symfile, "r") as f:
        syms = parse_syms(f)
    with open(args.infile, "r") as f:
        src = f.read()
    out = rewrite_syms(syms, src)
    with open(args.infile, "w") as f:
        f.write(out)


def parse_syms(f: TextIO) -> Symbols:
    syms: Symbols = {}
    for line in f:
        m = SYMBOL_PATTERN.match(line)
        if m is None:
            continue
        name, addr = m.group(1), m.group(2)
        syms[name] = addr
    return syms


def rewrite_syms(syms: Symbols, ins: str) -> str:
    for name, addr in syms.items():
        func_ident = f"func_{addr.upper()}"
        ins = ins.replace(func_ident, name)
    return ins


if __name__ == "__main__":
    main()
