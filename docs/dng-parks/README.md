# Parked candidates for DUNGEON.BIN's 116 carved objects

One file per function, named for the function: `func_b01_80172628.c`. The
names carry their unit's tag, so no two agents ever write the same path and
the directory needs no per-unit subdivision.

These are unit FRAGMENTS, not self-contained park sources: a body here needs
its unit's declaration block to compile. State those declarations in the
header, between `DECLARATIONS` and `END DECLARATIONS` markers, in the form
`docs/dungeon-parks/README.md` describes -- that convention is shared and the
reasons behind it are written up there.

Nothing here is built. The function is still an `INCLUDE_ASM` stub in
`csrc/ovl_dng_*.c`.

## A park here is usually a park for a WHOLE FAMILY

1199 of this block's 2575 stubs are exact copies of a sibling
(`tools/decomp/dngfam.py`). Before parking, run

    python3 tools/decomp/dngfam.py FUNC

and put the copy count in the header. A residue on a 46-copy representative
is worth 46 times what the word count says, and that is what decides whether
it is worth another day.
