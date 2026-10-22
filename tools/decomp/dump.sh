#!/bin/sh
: "${AZURE_ROOT:=$(cd "$(dirname "$0")/../.." && pwd)}"
: "${AZURE_SCRATCH:=/tmp/azure-decomp}"
mkdir -p "$AZURE_SCRATCH"
# usage: dump.sh file.c -> compiles with RTL dumps, copies them to scratchpad/dumps/<base>/
SP="$AZURE_SCRATCH"
T=$HOME/.cache/azure-psyq/drive_c/users/case/AppData/Local/Temp
b=$(basename "$1" .c)
rm -f $T/PQ*.sched $T/PQ*.sched2 $T/PQ*.rtl $T/PQ*.lreg $T/PQ*.greg $T/PQ*.jump $T/PQ*.cse $T/PQ*.flow $T/PQ*.combine $T/PQ*.jump2 $T/PQ*.cse2 $T/PQ*.loop
mkdir -p $SP/dumps/$b
cd "$AZURE_ROOT"
tools/ccpsx -O2 -G8 -Iinclude -da -S -o $SP/dumps/$b/out.s "$1" 2>&1
for f in $T/PQ*.*; do [ -f "$f" ] && cp "$f" $SP/dumps/$b/$(echo $f | sed 's/.*PQ[0-9]*\.//'); done
ls $SP/dumps/$b
