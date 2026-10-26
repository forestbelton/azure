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
tools/ccpsx -O2 -G8 -Iinclude -Ipsyq/include -S -o "$out" csrc/"$1".c >/dev/null
# cc1psx runs under wine, so the listing has CRLF line endings; strip the CR
# before matching or the function label never compares equal.
tr -d '\r' < "$out" | awk -v f="$2:" '$1==f{p=1} p{print} p&&/\.end/{exit}'
