import pathlib
from typing import Any

from splat.segtypes import segment
from splat.util import options

FILE_TEMPLATE = """.include "macro.inc"

.section .rodata

dlabel {name}
    .shiftjis "{data}"
    .fill {trailing_zeros}, 1, 0
"""


class ShiftJisException(Exception):
    base_exception: Exception | None

    def __init__(self, start_addr: int, base_exception: Exception | None = None):
        self.message = f"failed to parse Shift-JIS string at 0x{start_addr:08x}"
        self.base_exception = base_exception
        if base_exception is not None:
            self.message = f"{self.message}: {str(base_exception)}"
        super().__init__(self.message)

    def __str__(self):
        return self.message


def escape(data: str) -> str:
    """Escape a decoded string so the `.shiftjis` directive stays on one line.

    The item and dialogue pools embed the script's control bytes -- 0x0a for a
    line break, and 0x11/0x08 which the text engine uses for its own purposes
    -- and those decode to C0 control characters. Written raw they would end
    the directive's line mid-string and the assembler would see garbage.
    `azure shiftjis` reverses exactly these three escapes.
    """
    out: list[str] = []
    for ch in data:
        if ch == "\\":
            out.append("\\\\")
        elif ch == '"':
            out.append('\\"')
        elif ord(ch) < 0x20 or ord(ch) == 0x7F:
            out.append("\\x{:02x}".format(ord(ch)))
        else:
            out.append(ch)
    return "".join(out)


# NB: Pretty sure the game uses some special byte sequences that aren't valid
# Shift-JIS, will need to expand this (and rename) later when those arise.
class PSXSegShiftjis(segment.Segment):
    def __init__(
        self,
        rom_start: int | None,
        rom_end: int | None,
        type: str,
        name: str,
        vram_start: int | None,
        args: list[Any],
        yaml: Any,
        bss_size: int | None = None,
    ):
        # `bss_size` was added to Segment.__init__ in later splat releases and
        # is passed by keyword to every segment class, so it must be accepted
        # and forwarded even though a Shift-JIS string segment never has bss.
        if name == "" and rom_start is not None:
            name = hex(rom_start)
        super().__init__(  # type: ignore
            rom_start,
            rom_end,
            type,
            name,
            vram_start,
            args=args,
            yaml=yaml,
            bss_size=bss_size,
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

    def out_path(self) -> pathlib.Path | None:
        return options.opts.asm_path / f"{self.name}.s"

    def split(self, rom_bytes: bytes) -> None:
        encoded = bytes(rom_bytes[self.rom_start : self.rom_end])
        trailing_zeros = 0
        assert self.rom_end is not None
        while rom_bytes[self.rom_end - trailing_zeros - 1] == 0:
            trailing_zeros += 1
        assert trailing_zeros < len(encoded)
        if trailing_zeros > 0:
            encoded = encoded[:-trailing_zeros]
        out_path = self.out_path()
        assert out_path is not None
        out_path.parent.mkdir(parents=True, exist_ok=True)
        name = self.name.replace("/", "_").upper()
        try:
            data = encoded.decode("shift-jis")
        except Exception as exc:
            assert self.rom_start is not None
            raise ShiftJisException(self.rom_start, exc)
        with open(out_path, "w") as f:
            f.write(
                FILE_TEMPLATE.format(
                    name=name,
                    data=escape(data),
                    trailing_zeros=trailing_zeros,
                )
            )
