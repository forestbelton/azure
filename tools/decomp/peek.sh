#!/bin/sh
# peek.sh UNIT FUNC -- compile one csrc unit and print just that function's asm.
#
# Useful when the function is already in csrc/ and you want to see what it
# compiles to. For a candidate that is not in csrc/ yet, use verify.py, which
# also compares against the reference bytes.
: "${AZURE_ROOT:=$(cd "$(dirname "$0")/../.." && pwd)}"
: "${AZURE_SCRATCH:=/tmp/azure-decomp}"
mkdir -p "$AZURE_SCRATCH"
set -e
cd "$AZURE_ROOT"
out="$AZURE_SCRATCH"/$1.s
tools/cdkgcc -O2 -G8 -Iinclude -Ipsyq/include -S -o "$out" csrc/"$1".c >/dev/null
# The CR strip is a no-op for the native compiler, which emits LF. It is kept
# because it costs nothing and cc1psx under wine emitted CRLF, which made the
# function label never compare equal.
tr -d '\r' < "$out" | awk -v f="$2:" '$1==f{p=1} p{print} p&&/\.end/{exit}'
