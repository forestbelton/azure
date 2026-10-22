#!/usr/bin/env python3
"""Generate the preprocessed context file that m2c and decomp-permuter read.

Both tools want a single already-preprocessed C file holding every type,
struct and function declaration. Without it m2c emits `void *arg0` and
`->unk14` for every field access, which is correct but not something you can
usefully turn into matching C.

M2CTX is defined so include_asm.h compiles its INCLUDE_ASM macros away: the
real definitions expand to __asm__ statements, which pycparser cannot parse.
"""

import argparse
import pathlib
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent


def preprocess(headers: list[pathlib.Path], cpp: str) -> str:
    # One synthetic translation unit including everything, so a single pass
    # yields the whole project's declarations.
    unit = "".join(f'#include "{h.relative_to(ROOT)}"\n' for h in headers)
    cmd = [
        cpp, "-E", "-P",
        "-DM2CTX",
        "-D__GNUC__=2",
        "-undef",
        "-nostdinc",
        f"-I{ROOT / 'include'}",
        f"-I{ROOT}",
        "-x", "c", "-",
    ]
    proc = subprocess.run(cmd, input=unit, capture_output=True, text=True)
    if proc.returncode != 0:
        sys.stderr.write(proc.stderr)
        raise SystemExit(f"m2ctx: {cpp} failed")
    return proc.stdout


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("-o", "--output", type=pathlib.Path,
                    default=ROOT / "ctx.c")
    ap.add_argument("--cpp", default="cc",
                    help="host preprocessor (default: cc)")
    args = ap.parse_args()

    headers = sorted((ROOT / "include").glob("*.h"))
    if not headers:
        raise SystemExit("m2ctx: no headers found under include/")

    text = preprocess(headers, args.cpp)
    args.output.write_text(text)
    decls = sum(1 for l in text.splitlines() if l.strip())
    print(f"wrote {args.output.relative_to(ROOT)} from "
          f"{len(headers)} header(s), {decls} non-blank lines")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
