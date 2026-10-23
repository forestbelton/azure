#!/bin/bash
# Convert PSY-Q libraries to GNU ar archives for introspection.
#
#   tools/lib2ar.sh psyq/lib/libgpu.lib [more.lib ...]
#
# Produces libgpu.a in $OUTDIR (default: the current directory), which
# `nm`/`objdump` can read. Used to identify which library modules the game's
# executable links -- see docs/where-we-are.md on the unidentified code in
# slus_006.14, and `azure libmatch` / `azure libscan` for the matching itself.
#
# TODO: convert this to Python, where the .OBJ parsing could be done in-process
# by src/azure/psyobj.py instead of shelling out to psyq-obj-parser.
set -euo pipefail

: "${AZURE_ROOT:=$(cd "$(dirname "$0")/.." && pwd)}"
: "${AZURE_PATH:=$AZURE_ROOT/src/azure/main.py}"
: "${OUTDIR:=$PWD}"
: "${PYTHON:=python3}"
# src/ must be on the path or `main.py` cannot import its own package --
# the same reason the Makefile runs it as `PYTHONPATH=src python3 ...`.
export PYTHONPATH="$AZURE_ROOT/src${PYTHONPATH:+:$PYTHONPATH}"

# GNU ar is format-agnostic for `rcs` -- it only archives the members -- so a
# plain `ar` is fine here and a mips-targeted one is not required. Try the
# cross-ars first anyway, since a toolchain that has one is the likelier intent.
if [ -z "${AR:-}" ]; then
    for cand in mipsel-none-elf-ar mips-linux-gnu-ar mipsel-linux-gnu-ar ar; do
        if command -v "$cand" >/dev/null 2>&1; then AR=$cand; break; fi
    done
fi
if [ -z "${AR:-}" ]; then
    echo "lib2ar: no ar found; set AR=/path/to/ar" >&2
    exit 1
fi

# An external dependency, from pcsx-redux's tools. Not vendored here.
: "${PSYQ_OBJ_PARSER:=psyq-obj-parser}"
if ! command -v "$PSYQ_OBJ_PARSER" >/dev/null 2>&1 && [ ! -x "$PSYQ_OBJ_PARSER" ]; then
    cat >&2 <<MSG
lib2ar: cannot find psyq-obj-parser ("$PSYQ_OBJ_PARSER").

It converts a PSY-Q .OBJ to ELF and is not part of this repo. Note you do NOT
need it to identify library modules -- 'azure libmatch' and 'azure libscan'
read .lib files natively through src/azure/psyobj.py. It is only for turning a
library into an ar archive for nm/objdump.

To build it: in a pcsx-redux checkout, 'make bins/Release/psyq-obj-parser'.
That link FAILS on a distro without static libs ("cannot find -lz ... have you
installed the static version"), because the Makefile's TOOLDEF rule hardcodes
'-static -lz' for every tool. Either install the static libs (on Fedora:
glibc-static, libstdc++-static, zlib-static) or relink dynamically without
touching the vendored tree -- the objects are already built, only the link
failed, so take the link line make would run, drop -static, and rerun it:

    make -n bins/Release/psyq-obj-parser | grep 'psyq-obj-parser' \
        | grep 'g++' | tail -1 | sed 's/ -static / /g' | bash

Then put it on PATH or point at it:

    PSYQ_OBJ_PARSER=/path/to/psyq-obj-parser tools/lib2ar.sh "\$@"

Without it, \`PYTHONPATH=src $PYTHON $AZURE_PATH lib --list <lib>\` still lists a library's
modules and their externals, which is enough for most identification work.
MSG
    exit 1
fi

if [ "$#" -eq 0 ]; then
    echo "usage: tools/lib2ar.sh <library.lib> [...]" >&2
    exit 2
fi

mkdir -p "$OUTDIR"
OUTDIR=$(cd "$OUTDIR" && pwd)

for arg in "$@"; do
    if [ ! -f "$arg" ]; then
        echo "lib2ar: no such file: $arg" >&2
        exit 1
    fi
    INPATH=$(cd "$(dirname "$arg")" && pwd)/$(basename "$arg")

    # Strip ANY extension, in any case. The old `basename "$1" .LIB` assumed
    # uppercase and silently produced "libgpu.lib.a" for this repo's
    # lowercase psyq/lib/*.lib.
    LIB=$(basename "$arg"); LIB=${LIB%.*}

    LIBDIR=$(mktemp -d)
    trap 'rm -rf "$LIBDIR"' EXIT

    (
        cd "$LIBDIR"
        "$PYTHON" "$AZURE_PATH" lib --extract "$INPATH" >/dev/null

        # The extractor writes "<module>.OBJ", but module names come from the
        # library, so do not assume the case of either part.
        shopt -s nullglob nocaseglob
        objs=(*.obj)
        if [ ${#objs[@]} -eq 0 ]; then
            echo "lib2ar: $INPATH yielded no object modules" >&2
            exit 1
        fi
        for obj in "${objs[@]}"; do
            "$PSYQ_OBJ_PARSER" "$obj" -o "${obj%.*}.o" >/dev/null
        done
        "$AR" rcs "$LIB.a" *.o
    )
    cp "$LIBDIR/$LIB.a" "$OUTDIR/"
    echo "lib2ar: $arg -> $OUTDIR/$LIB.a"

    rm -rf "$LIBDIR"
    trap - EXIT
done
