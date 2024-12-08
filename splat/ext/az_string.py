import pathlib
from typing import Optional

from splat.segtypes import segment
from splat.util import options

FILE_TEMPLATE = """
.include "macro.inc"

.set noat
.set noreorder

.section .rodata

glabel {name}
{data}
"""

RAWTAB = {
    # NB: I'm defining these special values by convention. They should be moved
    # to macro definitions at some point to be more portable...
    "0x0": "\\x00",
    "0x3": "\\x03",
    "0x8": "\\x08",
    "0xa": "\\n",
    "0x11": "\\x11",
    "0x51": "\\x12",
    "0xfe00": "\\x13", # Koh
    "0xfe01": "\\x14", # Kewne
    "0x8140": " ",
    "0x8143": ",",
    "0x8144": ".",
    "0x8145": "♥",
    "0x8146": ":",
    "0x8147": ";",
    "0x8148": "?",
    "0x8149": "!",
    "0x814f": "^",
    "0x8150": "♥",
    "0x8151": "_",
    "0x815e": "/",
    "0x815f": "♥",
    "0x8160": "~",
    "0x8161": "♥",
    "0x8162": "|",
    "0x8163": "♥",
    "0x8164": "♥",
    "0x8165": "`",
    "0x8166": "'",
    "0x8167": "♥",
    "0x8168": '"',
    "0x8169": "(",
    "0x816a": ")",
    "0x816b": "♥",
    "0x816c": "♥",
    "0x816d": "[",
    "0x816e": "]",
    "0x816f": "{",
    "0x8170": "}",
    "0x817b": "+",
    "0x817c": "-",
    "0x817d": "♥",
    "0x817e": "Χ",
    "0x817f": "♥",
    "0x8180": "♥",
    "0x8181": "=",
    "0x8182": "♥",
    "0x8183": "<",
    "0x8184": ">",
    "0x8190": "$",
    "0x8191": "♥",
    "0x8192": "♥",
    "0x8193": "%",
    "0x8194": "#",
    "0x8195": "&",
    "0x8196": "х",
    "0x8197": "@",
    "0x819b": "○",
    "0x81a0": "□",
    "0x81a1": "♥",
    "0x81a2": "∆",
    "0x824f": "0",
    "0x8250": "1",
    "0x8251": "2",
    "0x8252": "3",
    "0x8253": "4",
    "0x8254": "5",
    "0x8255": "6",
    "0x8256": "7",
    "0x8257": "8",
    "0x8258": "9",
    "0x8260": "A",
    "0x8261": "B",
    "0x8262": "C",
    "0x8263": "D",
    "0x8264": "E",
    "0x8265": "F",
    "0x8266": "G",
    "0x8267": "H",
    "0x8268": "I",
    "0x8269": "J",
    "0x826a": "K",
    "0x826b": "L",
    "0x826c": "M",
    "0x826d": "N",
    "0x826e": "O",
    "0x826f": "P",
    "0x8270": "Q",
    "0x8271": "R",
    "0x8272": "S",
    "0x8273": "T",
    "0x8274": "U",
    "0x8275": "V",
    "0x8276": "W",
    "0x8277": "X",
    "0x8278": "Y",
    "0x8279": "Z",
    "0x8281": "a",
    "0x8282": "b",
    "0x8283": "c",
    "0x8284": "d",
    "0x8285": "e",
    "0x8286": "f",
    "0x8287": "g",
    "0x8288": "h",
    "0x8289": "i",
    "0x828a": "j",
    "0x828b": "k",
    "0x828c": "l",
    "0x828d": "m",
    "0x828e": "n",
    "0x828f": "o",
    "0x8290": "p",
    "0x8291": "q",
    "0x8292": "r",
    "0x8293": "s",
    "0x8294": "t",
    "0x8295": "u",
    "0x8296": "v",
    "0x8297": "w",
    "0x8298": "x",
    "0x8299": "y",
    "0x829a": "z",
}

TAB = {int(code, 16): sym for code, sym in RAWTAB.items()}

MAX_LINE_LEN = 80
LINE_START = '    .az_string "'


def decode(raw_str: bytes) -> bytes:
    print(raw_str.hex())

    i = 0
    lines = []
    line = [LINE_START]
    line_len = len(LINE_START)
    while i < len(raw_str):
        i0 = i
        code = raw_str[i]
        i += 1
        if code in {0x81, 0x82, 0xFE}:
            if i == len(raw_str):
                raise Exception("string ended unexpectedly")
            code = (code << 8) | raw_str[i]
            i += 1
        if code not in TAB:
            raise Exception(f"unexpected control byte {code=} at position {i0}")
        token = TAB[code]
        # wrap to next line
        if line_len + len(token) + 2 > MAX_LINE_LEN:
            line.append('"')
            lines.append("".join(line))
            line = [LINE_START]
            line_len = len(LINE_START)
        line.append(token)
        line_len += len(token)
    # last line
    if len(line) > 1:
        line.append('"')
        lines.append("".join(line))
    return "\n".join(lines)

# NB: This appears to just be ShiftJIS?
class PSXSegAz_string(segment.Segment):
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
        options.opts.asm_path.mkdir(parents=True, exist_ok=True)
        with open(self.out_path(), "w") as f:
            f.write(FILE_TEMPLATE.format(name=self.name, data=decode(encoded)))
