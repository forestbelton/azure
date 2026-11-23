#!/usr/bin/env python3
"""Put a landed carved function back to its INCLUDE_ASM stub.

The sweep needs this when a body VERIFIES but the CONTAINER still will not
build -- which is a real and separate failure. `ovlverify` measures a
function's own words and nothing else, so a body that makes gcc emit its own
jump table into `.rodata` reports `0 differing` and then grows the container
by the size of that table. `func_b03_80170E54`'s family did exactly that:
six members, 0x30 bytes of new `.rodata` each, +0x120 on dungeon.bin, and the
built code loading its own table at 0x80173C80 where the ROM loads
`jtbl_b03_80170808`.

Removing the definition is therefore not a retreat from a wrong body. It
parks a RIGHT body whose placement is not solved yet, and the park file keeps
it until the segment can give the compiler the ROM's table.
"""
import argparse
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dngclone  # noqa: E402

PROJ = dngclone.PROJ


def unsplice(func, keep_dir=None):
    path = dngclone.unit_path(func)
    unit = os.path.basename(path)[:-2]
    text = open(path).read()
    body, span = dngclone.definition(text, func)
    if body is None:
        return None
    if keep_dir:
        os.makedirs(keep_dir, exist_ok=True)
        with open(os.path.join(keep_dir, func + ".c"), "w") as fh:
            fh.write(body + "\n")
    stub = 'INCLUDE_ASM("asm/nonmatchings/%s", %s);' % (unit, func)
    open(path, "w").write(text[:span[0]] + stub + text[span[1]:])
    return unit, len(body.splitlines())


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("funcs", nargs="+")
    ap.add_argument("--keep", help="directory to save each removed body in")
    a = ap.parse_args()
    for f in a.funcs:
        r = unsplice(f, a.keep)
        print(f"{f}: {'removed from ' + r[0] if r else 'not defined -- skipped'}")


if __name__ == "__main__":
    main()
