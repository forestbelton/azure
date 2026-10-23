#!/bin/sh
: "${AZURE_ROOT:=$(cd "$(dirname "$0")/../.." && pwd)}"
: "${AZURE_SCRATCH:=/tmp/azure-decomp}"
mkdir -p "$AZURE_SCRATCH"
# usage: dump.sh file.c -> compiles with RTL dumps, copies them to scratchpad/dumps/<base>/
SP="$AZURE_SCRATCH"

# cc1psx writes its dump files into the wine prefix's Temp directory, so this
# MUST agree with tools/ccpsx's own choice of prefix. It honours
# AZURE_WINEPREFIX; resolve it the same way here. Getting this wrong does not
# fail loudly -- it silently collects stale dumps, or a concurrently running
# compile's dumps from the shared prefix, and attributes them to this source.
PREFIX=${AZURE_WINEPREFIX:-${XDG_CACHE_HOME:-$HOME/.cache}/azure-psyq}
T=""
for d in "$PREFIX"/drive_c/users/*/AppData/Local/Temp; do
    [ -d "$d" ] && T="$d" && break
done
if [ -z "$T" ]; then
    echo "dump.sh: no wine Temp dir under $PREFIX" >&2
    echo "  (looked for $PREFIX/drive_c/users/*/AppData/Local/Temp)" >&2
    exit 1
fi

b=$(basename "$1" .c)
# Wipe EVERY PQ* dump, not a list of known suffixes. cc1psx emits 16 of them --
# rtl jump cse loop cse2 flow combine regmove lreg greg sched sched2 bp dbr
# shorten -- and three of those (regmove, bp, shorten) are NOT in upstream
# 2.7.2's toplev.c suffix list, so an allowlist written from upstream silently
# leaves them behind and the next run collects a PREVIOUS function's dump under
# this function's name. That happened: a .regmove collected for one function
# was headed ";; Function func_80041344".
rm -f "$T"/PQ*.*

mkdir -p "$SP/dumps/$b"
cd "$AZURE_ROOT"
tools/ccpsx -O2 -G8 -Iinclude -da -S -o "$SP/dumps/$b/out.s" "$1" 2>&1
for f in "$T"/PQ*.*; do
    [ -f "$f" ] && cp "$f" "$SP/dumps/$b/$(echo "$f" | sed 's/.*PQ[0-9]*\.//')"
done
ls "$SP/dumps/$b"
