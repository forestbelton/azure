import pathlib
from typing import Optional

from splat.segtypes import segment
from splat.util import options

FILE_TEMPLATE = """.include "macro.inc"

.section .rodata

dlabel {name}
    .shiftjis "{data}"
    .fill {trailing_zeros}, 1, 0
"""

# NB: Pretty sure the game uses some special byte sequences that aren't valid
# Shift-JIS, will need to expand this (and rename) later when those arise.
class PSXSegShiftjis(segment.Segment):
    def __init__(
        self,
        rom_start: Optional[int],
        rom_end: Optional[int],
        type: str,
        name: str,
        vram_start: Optional[int],
        args: list,
        yaml,
    ):
        super().__init__(
            rom_start,
            rom_end,
            type,
            name,
            vram_start,
            args=args,
            yaml=yaml,
        )

    @staticmethod
    def is_noload() -> bool:
        return False

    @staticmethod
    def is_rodata() -> bool:
        return True

    def get_linker_section_order(self) -> str:
        return ".rodata"

    def get_linker_section_linksection(self) -> str:
        return ".rodata"

    def out_path(self) -> Optional[pathlib.Path]:
        return options.opts.asm_path / f"{self.name}.s"

    def split(self, rom_bytes: bytes) -> None:
        encoded = bytes(rom_bytes[self.rom_start : self.rom_end])
        trailing_zeros = 0
        while rom_bytes[self.rom_end - trailing_zeros - 1] == 0:
            trailing_zeros += 1
        assert trailing_zeros < len(encoded)
        if trailing_zeros > 0:
            encoded = encoded[:-trailing_zeros]
        self.out_path().parent.mkdir(parents=True, exist_ok=True)
        name = self.name.replace("/", "_").upper()
        with open(self.out_path(), "w") as f:
            f.write(
                FILE_TEMPLATE.format(
                    name=name,
                    data=encoded.decode("shift-jis"),
                    trailing_zeros=trailing_zeros,
                )
            )
