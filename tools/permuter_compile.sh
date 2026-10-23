#!/usr/bin/env bash
# Full compile pipeline for decomp-permuter, producing a real object.
#
# import.py derives its compile command from the Makefile and picks up only the
# first step, `ccpsx -S`, which emits assembly -- it then writes that text to
# the .o path and every objdump fails with "file format not recognized". Doing
# the whole ccpsx -> maspsx -> as pipeline here also means the permuter scores
# against the same toolchain the real build uses.
set -euo pipefail

INPUT="$(realpath "$1")"
OUTPUT="$(realpath "$3")"
# import.py copies this script into nonmatchings/<func>/, so its own location
# is not the project root. Walk up until the root is recognisable instead.
PROJ="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
while [ "$PROJ" != "/" ] && [ ! -x "$PROJ/tools/ccpsx" ]; do
    PROJ="$(dirname "$PROJ")"
done
if [ ! -x "$PROJ/tools/ccpsx" ]; then
    echo "permuter_compile.sh: cannot locate the project root" >&2
    exit 1
fi

MASPSX_DIR="vendor/maspsx"
if [ ! -f "$PROJ/$MASPSX_DIR/maspsx.py" ]; then
    echo "permuter_compile.sh: cannot locate vendored maspsx" >&2
    exit 1
fi

AS="${AS:-mips64-linux-gnu-as}"

cd "$PROJ"
S="$(mktemp --suffix=.s)"
trap 'rm -f "$S"' EXIT

tools/ccpsx -O2 -G8 -Iinclude -DPERMUTER -S "$INPUT" -o "$S"
python3 "$MASPSX_DIR/maspsx.py" \
    --aspsx-version=2.56 --force-stdin --run-assembler \
    --gnu-as-path="$AS" --use-comm-section -G8 \
    -EL -32 -march=r3000 -mtune=r3000 -no-pad-sections -O1 \
    -o "$OUTPUT" < "$S"
