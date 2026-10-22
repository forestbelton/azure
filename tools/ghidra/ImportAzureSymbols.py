# Import Azure Dreams symbols and fix up $gp.
#
# Run from Ghidra's Script Manager after importing data/slus_006.14 with the
# ghidra_psx_ldr loader. Generate the input first with:
#
#     python3 tools/ghidra_syms.py      # -> build/ghidra_symbols.tsv
#
# Two jobs. First, apply the 460 known symbols, 426 of which are functions, so
# the listing arrives named instead of blank. Second, set $gp to 0x80080994.
# That one matters: the PS-EXE header's gp0 field is 0x00000000, so Ghidra
# assumes there is no $gp and leaves every $gp-relative access unresolved --
# and there are 245 of them in the game's own code. crt0 sets the real value
# at runtime (lui/addiu at 0x800339ac), which a static load cannot see.
#
# @category Azure
# @runtime PyGhidra

import os

from java.math import BigInteger
from ghidra.program.model.symbol import SourceType

GP_VALUE = 0x80080994


# PSX addresses live at 0x8xxxxxxx, which is past Java's signed int range. A
# plain toAddr(pyint) makes jpype pick the toAddr(int) overload and raise
# OverflowError, so go through the address space with a hex string instead.
def addr_of(value):
    space = currentProgram.getAddressFactory().getDefaultAddressSpace()
    return space.getAddress("%x" % value)


def read_table(path):
    rows = []
    with open(path) as fh:
        for line in fh:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            parts = line.split("\t")
            if len(parts) != 3:
                continue
            rows.append((parts[0], int(parts[1], 16), parts[2]))
    return rows


def apply_symbols(rows):
    made_fn = renamed_fn = labelled = skipped = 0
    for name, value, kind in rows:
        if name == "_gp":
            continue
        addr = addr_of(value)
        if getMemoryBlock(addr) is None:
            skipped += 1          # outside the loaded image
            continue
        if kind == "func":
            fn = getFunctionAt(addr)
            if fn is None:
                if createFunction(addr, name) is not None:
                    made_fn += 1
                else:
                    # Undefined or mid-instruction: fall back to a label so
                    # the address is still annotated rather than silently lost.
                    createLabel(addr, name, True)
                    labelled += 1
            else:
                fn.setName(name, SourceType.USER_DEFINED)
                renamed_fn += 1
        else:
            createLabel(addr, name, True)
            labelled += 1
    return made_fn, renamed_fn, labelled, skipped


def set_gp():
    gp = currentProgram.getRegister("gp")
    if gp is None:
        print("warning: no $gp register for this language; skipping")
        return 0
    ctx = currentProgram.getProgramContext()
    n = 0
    for block in currentProgram.getMemory().getBlocks():
        if not block.isInitialized():
            continue
        ctx.setValue(gp, block.getStart(), block.getEnd(),
                     BigInteger("%x" % GP_VALUE, 16))
        n += 1
    createLabel(addr_of(GP_VALUE), "_gp", True)
    return n


def find_table():
    """Locate the TSV without a dialog where possible.

    A dialog makes the script unusable headlessly, which is also how it gets
    tested. Order: an explicit script argument, then the conventional path
    relative to this script, then ask.
    """
    args = getScriptArgs()
    if args:
        return args[0]
    try:
        here = os.path.dirname(os.path.abspath(
            str(sourceFile.getAbsolutePath())))
        guess = os.path.join(here, "..", "..", "build", "ghidra_symbols.tsv")
        guess = os.path.normpath(guess)
        if os.path.isfile(guess):
            print("using " + guess)
            return guess
    except Exception:
        pass
    return askFile("Azure symbol table (build/ghidra_symbols.tsv)",
                   "Import").getAbsolutePath()


rows = read_table(find_table())
print("read %d symbols" % len(rows))

made, renamed, labelled, skipped = apply_symbols(rows)
print("functions created: %d" % made)
print("functions renamed: %d" % renamed)
print("labels applied:    %d" % labelled)
if skipped:
    print("skipped (outside loaded memory): %d" % skipped)

blocks = set_gp()
print("$gp = 0x%08x set over %d initialized block(s)" % (GP_VALUE, blocks))
print("done -- re-run Auto Analyze so $gp-relative references resolve")
