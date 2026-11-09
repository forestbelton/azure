import argparse
import re
import sys
from typing import TypedDict


class Func(TypedDict):
    ret: str
    params: str


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--funcs", required=True)
    parser.add_argument("--cfile", required=True)
    args = parser.parse_args()
    func_names: list[str] = []
    with open(args.funcs, "r") as f:
        func_names = [line.strip() for line in f]
    with open(args.cfile, "r") as f:
        csrc = f.read()
    funcs: dict[str, Func] = {}
    for func_name in func_names:
        pat = re.compile(
            r"^([^\s]+? \*?)" + func_name + r"\((.*?)\) *$", flags=re.MULTILINE
        )
        matches = pat.findall(csrc)
        if len(matches) != 1:
            print(
                f"error: unexpected number of matches for {func_name}: {len(matches)}",
                file=sys.stderr,
            )
            sys.exit(1)
        funcs[func_name] = {
            "ret": matches[0][0].strip(),
            "params": matches[0][1],
        }
    for name, func in funcs.items():
        print(f"{func['ret']} {name}({func['params']});")


if __name__ == "__main__":
    main()
