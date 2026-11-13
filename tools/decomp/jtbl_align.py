#!/usr/bin/env python3
"""Downgrade a jump table's `.align 3` to `.align 2`, in place.

WHY THIS EXISTS.  gcc aligns every jump table to BIGGEST_ALIGNMENT:

    final.c:1516   readonly_data_section ();
    final.c:1518   ASM_OUTPUT_ALIGN (file, exact_log2 (BIGGEST_ALIGNMENT
                                                       / BITS_PER_UNIT));

guarded only by `#ifdef READONLY_DATA_SECTION`, which mips.h defines (:3690),
and BIGGEST_ALIGNMENT is 64 (mips.h:1080) -- so `.align 3`, eight bytes.

That is RIGHT for the PSY-Q 4.1 units and must not be touched there.  The
live 0x80020000 overlay proves it: csrc/main/c_server.c's handler tables end
at rodata offset 0x5C, its jump table starts at 0x60, and those four pad
bytes are in the shipped image.  Dropping the directive globally would break
three containers.

It is WRONG for main.bin's 0x80400000 debug twin, the one block that came out
of the 4.0 SDK (see the Makefile).  Its jump table is at 0x8040007C, 4 mod 8,
with 0x7C of c_server.c's OWN rodata in front of it -- ASSERT_FMT and
"c_server.c", which no other translation unit can own.  An 8-aligning
toolchain cannot place a table there, so that block's did not 8-align.

Only the one directive is rewritten: a `.align 3` is downgraded when what
follows it is a label whose first datum is a `.word` naming a local label,
which is a jump table and nothing else.  An `.align 3` in front of a double
constant is left alone.
"""

import re
import sys

ALIGN3 = re.compile(r"^\s*\.align\s+3\s*$")
LABEL = re.compile(r"^\$?[A-Za-z_.$][\w.$]*:\s*$")
JTBL_WORD = re.compile(r"^\s*\.word\s+\$")


def main(path: str) -> int:
    with open(path) as fh:
        lines = fh.read().splitlines()

    hits = 0
    for i, line in enumerate(lines):
        if not ALIGN3.match(line):
            continue
        rest = [j for j in range(i + 1, len(lines)) if lines[j].strip()]
        if len(rest) < 2:
            continue
        if LABEL.match(lines[rest[0]].strip()) and JTBL_WORD.match(lines[rest[1]]):
            lines[i] = "\t.align\t2"
            hits += 1

    if hits:
        with open(path, "w") as fh:
            fh.write("\n".join(lines) + "\n")
    print(f"jtbl_align: {hits} jump-table .align 3 -> .align 2 in {path}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1]))
