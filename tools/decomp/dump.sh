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
# Worth keeping from that era: cc1psx emits these dumps, and three of them
# (regmove, bp, shorten) are NOT in upstream 2.7.2's toplev.c suffix list,
# which is itself evidence this is the SN compiler and not stock 2.7.2.
#
# CORRECTED TWICE ON 2026-11-04, and the second correction is the one to
# trust. This comment used to list the dumps as
#
#   rtl jump cse loop cse2 flow combine regmove lreg greg sched sched2 bp dbr
#   shorten jump2
#
# and assert "THAT LIST IS THE PASS ORDER". The list was wrong in three
# places and the assertion is wrong in one. Read both orders off the compiler
# source instead of trusting a remembered list; they are NOT the same order.
#
# WHERE THE DUMPS ARE OPENED -- toplev.c:2319-2389, in this order:
#
#   rtl jump cse loop cse2 bp flow combine regmove sched lreg greg sched2
#   jump2 dbr stack shorten
#
# WHAT ACTUALLY RUNS, AND WHEN -- the TIMEVAR calls in toplev.c:
#
#   branch_prob      :3412   (after cse2, BEFORE flow)
#   schedule_insns   :3548   (sched1, under the flag_schedule_insns guard :3543)
#   shorten_lifetimes:3568
#   local_alloc      :3585
#   global_alloc     :3617
#   schedule_insns   :3661   (sched2)
#   jump_optimize    :3690   (jump2)
#   dbr_schedule     :3723
#   reg_to_stack     :3748
#
# So: `.bp` sits between cse2 and flow, not after sched2. `jump2` runs BEFORE
# `dbr`, not after. And `.shorten` is opened LAST but EXECUTES between sched1
# and local-alloc -- which is the one place the open order and the run order
# genuinely disagree, so a list of suffixes cannot be read as a pass order at
# all.
#
# TWO FACTS WORTH CARRYING AWAY, both load-bearing in practice:
#
#   * sched1 RUNS BEFORE BOTH ALLOCATORS. Inside a single basic block the
#     INSN_LUID order that decides which quantity local-alloc hands $v0 is the
#     order sched1 has already emitted, so the two cannot be reasoned about
#     independently. Any argument of the form "local-alloc picks the registers
#     and then sched1 reorders them" has the passes backwards. The reverse
#     direction is fine and is what func_800B8E08's and func_800AD138's
#     landings rest on: sched1 sets the order, local-alloc then prices it.
#   * A dump's CONTENT is the check, not its name's position in any list.
#   * THE DUMPS ALSO TELL YOU WHICH SCHEDULER IS LINKED, and that matters
#     because scratch/gcc-src/cdk-gcc-b18 carries BOTH sched.c and
#     haifa-sched.c and Makefile.in:1447 builds one via $(SCHED_PREFIX)sched.c.
#     ";; ready list at T-" and ";; insn N has a greater potential hazard"
#     appear in sched.c and in haifa-sched.c NOT AT ALL, so cc1psx links
#     sched.c. Cite sched.c; anything found only in haifa-sched.c -- notably
#     INSN_REG_WEIGHT, at haifa-sched.c:288 -- belongs to a compiler this
#     project does not use, and one park cited it as a tie-break for years.
#     `.bp` still shows pseudos and pre-sched order, which is how its real
#     position was caught -- after this comment had already been corrected
#     once. If a dump looks mislabelled, read what is in it and then find the
#     TIMEVAR call, rather than arguing from the suffix list.
# table printed jump2 above sched2; jump2 runs LAST of the sixteen, so having
# fewer compares and jumps there than at sched2 is correct, not a defect. If a
# count seems to go the wrong way, check the order above first.

# -G: THE OVERLAYS BUILD AT -G0 AND THIS SCRIPT USED TO HARDCODE -G8.
# Every dump it collected for an overlay function was therefore from a
# DIFFERENT COMPILATION than the one being matched -- the same trap already
# recorded for helpers copied out of scratch/, sitting in the shared tool.
# Found 2026-11-03, after sched.py had been named in the generated brief as the
# way to read the scheduler's own decisions. Default is now -G0; pass a second
# argument to override (the resident's gprel_* units are -G8), and the value
# used is printed so it cannot be silently wrong again.
G="${2:--G0}"
case "$G" in -G*) ;; *) G="-G$G" ;; esac
echo "dump.sh: compiling with $G (overlays are -G0; pass a 2nd arg to override)" >&2

b=$(basename "$1" .c)
case "$1" in
    /*) in_abs="$1" ;;
    *)  in_abs="$AZURE_ROOT/$1" ;;
esac

rm -rf "$SP/dumps/$b"
mkdir -p "$SP/dumps/$b"
cd "$SP/dumps/$b"
"$AZURE_ROOT"/tools/cdkgcc -O2 "$G" -I"$AZURE_ROOT/include" -I"$AZURE_ROOT/psyq/include" \
    -da -S -o out.s "$in_abs" 2>&1

# Strip the input's name so callers see bare pass names (out.s stays out.s).
for f in "$b".c.*; do
    [ -f "$f" ] && mv "$f" "${f#"$b".c.}"
done
ls
