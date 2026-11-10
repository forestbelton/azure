"""A `.rodata` subsegment whose assembly file WE own.

splat ships `hasm` -- a code subsegment it seeds once and then never rewrites
(`segtypes/common/hasm.py`: `if out_path and not out_path.exists()`) -- but it
has no data or rodata counterpart. This is that counterpart: `hrodata` is to
`rodata` what `hasm` is to `asm`.

Use it on a run of data that should be hand-maintained assembly rather than
either splat's output or compiler output. A string table is the motivating
case: the bytes have to stay exact, but `.word 0xCA8BC882` blobs are unreadable
and a C literal cannot hold Shift-JIS text without octal escapes.

    - [0x848, hrodata, str_name_table]   # -> asrc/str_name_table.s

Three things make the file ours, and all three have to hold:

1. **splat emits the linker entry.** The type is NOT dotted, so
   `CommonSegData.out_path()` returns `asm_out_path()`, which this class
   redirects out of splat's tree. The generated `build/<container>.ld` then
   says `build/asrc/<name>.o(.rodata)`, which is the only way an object enters
   the link -- the `ld` command line passes no objects at all.
2. **splat never clobbers it.** `split_as_asm_file` below writes only when the
   file is absent, so the first `make splat` after adding the yaml entry seeds
   the file with spimdisasm's faithful disassembly and every later one leaves
   it alone. Nothing else about the split changes, which was measured rather
   than assumed: against the same subsegment typed plain `rodata`, the seeded
   body is byte-identical to splat's own output, the generated
   `undefined_syms_auto.txt` is identical, and the linker script differs only
   in the object path. The range also keeps being scanned on every split, so
   splat goes on knowing these symbols, their sizes and their types -- unlike
   the dotted `.rodata` route, which extracts nothing and so knows the range
   only through whatever references reach into it.
3. **`make` sees it.** `SFILES` in the Makefile globs `asrc` alongside `asm`,
   and the existing `build/%.o: %.s` rule does the rest, assembling at `-G0`
   with `-Iinclude` so `macro.inc` resolves and nothing can wander into small
   data.

   For a pool of text, do not keep the seeded `.word` blob and do not hand-write
   the assembly either. Name the subsegment `str/<name>`, so this class writes
   `asrc/str/<name>.s`, and put the strings in `asrc/str/<name>.txt`: the
   `asrc/str/%.s: asrc/str/%.txt` rule regenerates the `.s` from it with
   `tools/shiftjis_to_asm.py`, whose docstring has the entry forms. The `.s` is
   gitignored, the `.txt` is the source of truth, and `make splat` depends on
   the generated files so they exist before a split looks for them.

   Note what that means for `check_provenance` below: it reads the GENERATED
   file, so the `Covers rom` line has to be a comment in the `.txt`, which the
   generator passes through.

Default location is `asrc/`, a sibling of `csrc/`. splat's own `hasm`
convention would be `src_path` (here `csrc/`), and `hasm_in_src_path` is how it
spells that, but an `.s` next to the `.c` files is a trap in this repo: both
`build/%.o: %.s` and `build/%.o: %.c` would match `build/csrc/<name>.o`, and
which one make picks depends on rule order. A separate tree cannot be
ambiguous. Override per subsegment with the dict form if needed:

    - {start: 0x848, type: hrodata, name: str_name_table, path: rodata}

The seeded header records the rom range the bytes came from, and a later split
warns when the yaml has moved off it. That is the only guard against this
type's silent failure mode: shrink or grow the subsegment and the frozen file
still assembles, just no longer to the size the linker script is placing, which
otherwise surfaces only as a `make check` offset. The warning goes out through
`log.write` and not `Segment.warn`, because splat's `print_segment_warnings`
walks only the list `initialize_segments` returns -- top-level segments -- so a
warning registered on a subsegment is collected and never printed.
"""

import pathlib
import re
from typing import List, Optional

from splat.segtypes.common.rodata import CommonSegRodata
from splat.util import log, options

# The default tree for hand-owned assembly: `asrc` to `csrc` as `.s` to `.c`.
# NOT under `asm/`, which splat rewrites on every split and `make clean`
# deletes outright.
DEFAULT_PATH = pathlib.Path("asrc")

# Written into the seeded header, and re-read on every later split to check the
# yaml has not moved out from under the file. Deleting the line from the header
# turns the check off.
PROVENANCE_TEMPLATE = "Covers rom 0x{start:06X}..0x{end:06X}"
PROVENANCE_PATTERN = re.compile(r"Covers rom 0x([0-9A-Fa-f]+)\.\.0x([0-9A-Fa-f]+)")


class PSXSegHrodata(CommonSegRodata):
    def owned_path(self) -> pathlib.Path:
        """The tree this subsegment's file lives in, `asrc` unless overridden.

        `self.yaml` is the raw yaml entry, a list for the `[start, type, name]`
        form and a dict for the verbose one, so `path` is only available in the
        latter.
        """
        if isinstance(self.yaml, dict) and "path" in self.yaml:
            return pathlib.Path(str(self.yaml["path"]))
        return DEFAULT_PATH

    def asm_out_path(self) -> pathlib.Path:
        """Where the file lives -- and, through `CommonSegData.out_path()`, the
        object the linker script will reference.

        `options.opts.data_path` is already absolute (splat joins every
        configured path onto `base_path` when it parses the options), so the
        replacement has to be too, or `clean_up_path` in splat's linker writer
        cannot make it relative again for the `.ld` and the entry comes out as
        an absolute path that only builds on this machine.

        No `.rodata` in the stem: splat's own data files are
        `<name>.<type>.s`, which is how `str_pool_800.rodata.s` becomes
        `build/asm/data/str_pool_800.rodata.o`. A hand-owned file has no reason
        to carry the section in its name, so this is `<name>.s` ->
        `build/asrc/<name>.o`.
        """
        return (
            options.opts.base_path / self.owned_path() / self.dir / f"{self.name}.s"
        )

    def get_asm_file_extra_directives(self) -> List[str]:
        """The seeded file's header comment.

        Only ever written once, since the file is never regenerated, so this is
        a note to whoever opens it next rather than boilerplate. `/* */` is
        passed through untouched by `azure shiftjis` and understood by GNU as.
        """
        assert self.rom_start is not None
        assert self.rom_end is not None

        provenance = PROVENANCE_TEMPLATE.format(start=self.rom_start, end=self.rom_end)
        vram = ""
        if self.vram_start is not None:
            size = self.rom_end - self.rom_start
            vram = f" (vram 0x{self.vram_start:08X}..0x{self.vram_start + size:08X})"

        return [
            "/* HAND-OWNED .rodata. splat seeded this file once, from the ROM, and",
            " * will not rewrite it: the `hrodata` subsegment type exists to hand the",
            " * file over. Edit it freely -- `.shiftjis \"...\"` works here, because",
            " * `make` pipes every asm source through `azure shiftjis`. That filter is",
            " * line-anchored, so a `.shiftjis` directive must sit ALONE on its line:",
            " * leave the offset note on the line above it, or the directive reaches",
            " * GNU as unexpanded and `unknown pseudo-op` is what you get.",
            " *",
            f" * {provenance}{vram}.",
            " * Keep that size exact and keep the symbol names: the linker script",
            " * places this object's .rodata at the offset above, and other asm still",
            " * resolves these symbols by name. Moving the subsegment in the yaml does",
            " * NOT move the bytes here -- splat warns, and `make check` reports the",
            " * first offset that drifted.",
            " *",
            " * The `nonmatching` markers below are splat's note that a symbol is not",
            " * decompiled yet. Nothing in tools/ reads them; drop them as the data",
            " * here becomes real, named, understood data.",
            " */",
            "",
        ]

    def check_provenance(self, path: pathlib.Path) -> None:
        """Warn when the yaml range no longer matches what the file was seeded for."""
        try:
            header = path.read_text(encoding="utf-8")[:4096]
        except OSError as exc:
            log.write(
                f"hrodata: could not read hand-owned {path}: {exc}", status="warn"
            )
            return

        match = PROVENANCE_PATTERN.search(header)
        if match is None:
            # The header was rewritten or dropped. That is allowed -- it just
            # costs the check.
            return

        seeded = (int(match.group(1), 16), int(match.group(2), 16))
        current = (self.rom_start, self.rom_end)
        if seeded != current:
            assert self.rom_start is not None and self.rom_end is not None
            log.write(
                f"hrodata: hand-owned {path} was seeded for rom "
                f"0x{seeded[0]:06X}..0x{seeded[1]:06X}, but this subsegment is now "
                f"0x{self.rom_start:06X}..0x{self.rom_end:06X}. splat will not "
                f"reseed it, so the file's bytes no longer cover the range the "
                f"linker places them at. Fix the file by hand (or delete it and "
                f"re-split to reseed), then `make check`.",
                status="warn",
            )

    def split_as_asm_file(self, out_path: Optional[pathlib.Path]) -> None:
        """Seed the file if it is missing; otherwise leave it entirely alone.

        Same gate as `CommonSegHasm.split`, placed on the write rather than on
        `CommonSegData.split` so that the scan, the symbol creation and the
        subsegment group handling all still run exactly as they do for a plain
        `rodata` subsegment.
        """
        if out_path is not None and out_path.exists():
            self.check_provenance(out_path)
            self.log(f"preserving hand-owned {out_path}")
            return

        super().split_as_asm_file(out_path)

        if out_path is not None:
            log.write(
                f"hrodata: seeded {out_path} from the ROM -- it is yours now, "
                f"splat will not rewrite it",
                status="ok",
            )
