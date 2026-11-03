#!/bin/sh
: "${AZURE_ROOT:=$(cd "$(dirname "$0")/../.." && pwd)}"
: "${AZURE_SCRATCH:=/tmp/azure-decomp}"
mkdir -p "$AZURE_SCRATCH"
# usage: dump.sh file.c -> compiles with RTL dumps, copies them to scratchpad/dumps/<base>/
SP="$AZURE_SCRATCH"

# The compiler writes its -da dumps into the CURRENT DIRECTORY, named after the
# input file: gprel_148FC.c.greg, gprel_148FC.c.lreg and so on. So we compile
# from inside the dump directory, with absolute include paths because the
# project's own -I flags are relative to AZURE_ROOT.
#
# This used to be much worse. Under wine, cc1psx wrote the dumps into the wine
# prefix's Temp directory as PQ<number>.<pass>, which meant this script had to
# agree with tools/ccpsx about which prefix -- base or a pool one -- that run
# would claim, and getting it wrong silently collected a DIFFERENT function's
# dumps under this function's name. A .regmove collected for one function once
# arrived headed ";; Function func_80041344". The native compiler names the
# dumps after the input, so that failure mode is gone.
#
# Worth keeping from that era: cc1psx emits 16 dumps -- rtl jump cse loop cse2
# flow combine regmove lreg greg sched sched2 bp dbr shorten jump2 -- and three
# of them (regmove, bp, shorten) are NOT in upstream 2.7.2's toplev.c suffix
# list, which is itself evidence this is the SN compiler and not stock 2.7.2.

b=$(basename "$1" .c)
case "$1" in
    /*) in_abs="$1" ;;
    *)  in_abs="$AZURE_ROOT/$1" ;;
esac

rm -rf "$SP/dumps/$b"
mkdir -p "$SP/dumps/$b"
cd "$SP/dumps/$b"
"$AZURE_ROOT"/tools/cdkgcc -O2 -G8 -I"$AZURE_ROOT/include" -I"$AZURE_ROOT/psyq/include" \
    -da -S -o out.s "$in_abs" 2>&1

# Strip the input's name so callers see bare pass names (out.s stays out.s).
for f in "$b".c.*; do
    [ -f "$f" ] && mv "$f" "${f#"$b".c.}"
done
ls
