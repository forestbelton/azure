"""PSY-Q object (.OBJ) parser.

The record set and relocation semantics below were derived empirically from
the 8736 modules in the SDK releases under `psy-q/`, by correlating each
relocation against the instruction it patches:

    0x10  patches 32-bit data words      -> mask 0xffffffff
    0x1e  patches loads/stores only       -> mask 0x0000ffff  ($gp-relative;
          never paired with a hi16, which is what distinguishes it from 0x54)
    0x4a  patches `jal` / `j`            -> mask 0x03ffffff  (26-bit target)
    0x52  patches `lui`                  -> mask 0x0000ffff  (hi16)
    0x54  patches lw/sw/addiu/lhu/sh/lh  -> mask 0x0000ffff  (lo16)

Parsing is deliberately tolerant. A handful of modules (chiefly the libgcc
helpers in LIBSN and the LIBGS samples) carry debug records whose layout is
not covered here. Rather than fail, the parser stops at the first record it
does not understand and sets `truncated`.

A truncated parse is NOT safe to treat as complete: records are interleaved,
so stopping early can lose section contents entirely (which presents as an
empty `.text`) or drop relocations (which presents as a spurious mismatch).
Callers must surface the flag and treat an empty section as "unknown", never
as "different".
"""

import dataclasses
import struct
from typing import Optional

MAGIC = b"LNK\x02"

RELOC_MASKS: dict[int, int] = {
    0x10: 0xFFFFFFFF,  # 32-bit word
    0x1E: 0x0000FFFF,  # $gp-relative 16
    0x4A: 0x03FFFFFF,  # 26-bit jump target
    0x52: 0x0000FFFF,  # hi16 (lui)
    0x54: 0x0000FFFF,  # lo16
}


class PsyQObjException(Exception):
    pass


class UnknownExprKind(PsyQObjException):
    def __init__(self, kind: int) -> None:
        self.kind = kind
        super().__init__(f"unknown expression kind {kind:#02x}")


BINARY = -1  # marker: this expression kind takes two sub-expressions

# Kinds whose payload size is known from direct inspection of real objects.
EXPR_FIXED: dict[int, int] = {
    0x00: 4,  # literal u32
    0x02: 2,  # symbol index
    0x04: 2,  # section index
    0x0C: 2,
    0x16: 2,
    0x2A: 2,
}

# Operators confirmed by inspection: `2c <expr> <expr>`, and the nested form
# `32 <const> 2e <expr> <expr>` seen in libsn/SNMAIN.
EXPR_HINTS_BASE: dict[int, int] = {
    0x2C: BINARY,
    0x2E: BINARY,
    0x32: BINARY,
    0x38: BINARY,
}

# Candidate shapes tried for a kind that is still unknown, in order.
_EXPR_CANDIDATES = (BINARY, 2, 4, 1, 3, 0)


def _solve_expr_hints(data: bytes) -> dict[int, int]:
    """Infer the shape of unknown expression kinds from the module itself.

    Guessing an operator's arity is unsafe: get it wrong and the record stream
    desynchronises silently, which can over-broaden a relocation mask into a
    false match. So instead of assuming, each candidate shape is tested by
    re-parsing the *whole* module and requiring it to consume cleanly. A shape
    is adopted only when exactly one candidate does -- otherwise the kind is
    left unknown and the module is reported truncated, which is the safe
    failure.
    """
    hints: dict[int, int] = {}
    for _ in range(8):  # bounded: resolve at most 8 unknown kinds per module
        try:
            probe = PsyQObject.decode(data, expr_hints=hints)
        except PsyQObjException:
            return hints
        if not probe.truncated:
            return hints
        kind = probe.unknown_kind
        if kind is None or kind in hints:
            return hints
        viable = []
        for candidate in _EXPR_CANDIDATES:
            trial = dict(hints)
            trial[kind] = candidate
            try:
                result = PsyQObject.decode(data, expr_hints=trial)
            except PsyQObjException:
                continue
            if not result.truncated or result.unknown_kind not in (None, kind):
                viable.append(candidate)
        if len(viable) != 1:
            return hints  # ambiguous or impossible: leave it unknown
        hints[kind] = viable[0]
    return hints


@dataclasses.dataclass
class Reloc:
    type: int
    offset: int

    @property
    def mask(self) -> int:
        return RELOC_MASKS.get(self.type, 0xFFFFFFFF)

    @property
    def known(self) -> bool:
        return self.type in RELOC_MASKS


@dataclasses.dataclass
class Export:
    name: str
    section: int
    offset: int


@dataclasses.dataclass
class PsyQObject:
    sections: dict[str, bytes]
    relocs: dict[str, list[Reloc]]
    exports: list[Export]
    section_names: dict[int, str]
    truncated: bool
    stopped_at: Optional[str] = None
    unknown_kind: Optional[int] = None

    def text(self) -> Optional[bytes]:
        return self.sections.get(".text")

    def text_relocs(self) -> list[Reloc]:
        return self.relocs.get(".text", [])

    def text_exports(self) -> list[Export]:
        """Exports that live in .text, so can locate it.

        Linker-script symbols such as `__heapsize` or `__datalen` sit in .data
        or .sbss; using them to infer a .text base yields nonsense offsets.
        """
        return [
            e for e in self.exports if self.section_names.get(e.section) == ".text"
        ]

    @staticmethod
    def decode(
        data: bytes, expr_hints: Optional[dict[int, int]] = None
    ) -> "PsyQObject":
        if data[:4] != MAGIC:
            raise PsyQObjException(f"bad magic {data[:4]!r}")
        if expr_hints is None:
            expr_hints = _solve_expr_hints(data)
        hints = dict(EXPR_HINTS_BASE)
        hints.update(expr_hints)

        names: dict[int, str] = {}
        content: dict[int, bytearray] = {}
        # Relocation offsets are relative to the most recent contents record
        # for that section, NOT to the section start. A module whose .text is
        # emitted in several chunks reuses small offsets in each one, so they
        # must be rebased or they land on the wrong words (and, because masks
        # are OR-ed together, mask words that were never relocated).
        chunk_base: dict[int, int] = {}
        relocs: dict[int, list[Reloc]] = {}
        exports: list[Export] = []
        current: Optional[int] = None
        truncated = False
        stopped_at: Optional[str] = None
        unknown_kind: Optional[int] = None
        pos = 4

        def u8(p: int) -> tuple[int, int]:
            return data[p], p + 1

        def u16(p: int) -> tuple[int, int]:
            return struct.unpack_from("<H", data, p)[0], p + 2

        def u32(p: int) -> tuple[int, int]:
            return struct.unpack_from("<I", data, p)[0], p + 4

        def pstr(p: int) -> tuple[str, int]:
            n = data[p]
            return data[p + 1 : p + 1 + n].decode("ascii", "replace"), p + 1 + n

        def expr(p: int, depth: int = 0) -> int:
            """Skip a relocation expression, returning the position after it.

            Only the length matters here: a relocation's offset and type
            precede its expression, and masking needs nothing else. Unknown
            kinds are resolved by `_solve_expr_hints`, never guessed inline.
            """
            if depth > 24:
                raise PsyQObjException("relocation expression too deep")
            kind, p = u8(p)
            if kind in EXPR_FIXED:
                return p + EXPR_FIXED[kind]
            arity = hints.get(kind)
            if arity is None:
                raise UnknownExprKind(kind)
            if arity == BINARY:
                return expr(expr(p, depth + 1), depth + 1)
            return p + arity

        while pos < len(data):
            try:
                tag, pos = u8(pos)
                if tag == 0x00:  # end of module
                    break
                elif tag == 0x02:  # section contents
                    n, pos = u16(pos)
                    if current is not None:
                        buf = content.setdefault(current, bytearray())
                        chunk_base[current] = len(buf)
                        buf += data[pos : pos + n]
                    pos += n
                elif tag == 0x06:  # switch to section
                    current, pos = u16(pos)
                elif tag == 0x08:  # advance section by uninitialised space
                    n, pos = u32(pos)
                    if current is not None:
                        content.setdefault(current, bytearray())
                        content[current] += b"\x00" * n
                elif tag == 0x0A:  # relocation
                    rtype, pos = u8(pos)
                    offset, pos = u16(pos)
                    pos = expr(pos)
                    if current is not None:
                        relocs.setdefault(current, []).append(
                            Reloc(
                                type=rtype,
                                offset=offset + chunk_base.get(current, 0),
                            )
                        )
                elif tag == 0x0C:  # exported (defined) symbol
                    _, pos = u16(pos)
                    section, pos = u16(pos)
                    offset, pos = u32(pos)
                    name, pos = pstr(pos)
                    exports.append(Export(name=name, section=section, offset=offset))
                elif tag == 0x0E:  # imported (undefined) symbol
                    _, pos = u16(pos)
                    _, pos = pstr(pos)
                elif tag == 0x10:  # section definition
                    index, pos = u16(pos)
                    _, pos = u16(pos)  # group
                    _, pos = u8(pos)  # alignment
                    name, pos = pstr(pos)
                    names[index] = name
                elif tag in (0x12, 0x30):  # local bss symbol: idx, section,
                    _, pos = u16(pos)      # size, name (e.g. libgpu's _que,
                    _, pos = u16(pos)      # 0x1800 bytes)
                    _, pos = u32(pos)
                    _, pos = pstr(pos)
                elif tag == 0x1C:  # source file name: index, path
                    _, pos = u16(pos)
                    _, pos = pstr(pos)
                elif tag == 0x1E:  # function end (debug)
                    pos += 2 + 4 + 4
                elif tag == 0x2E:  # processor type
                    _, pos = u8(pos)
                else:
                    raise PsyQObjException(f"unknown tag {tag:#02x} at {pos - 1:#x}")
            except (PsyQObjException, struct.error, IndexError) as exc:
                truncated = True
                stopped_at = str(exc)
                if isinstance(exc, UnknownExprKind):
                    unknown_kind = exc.kind
                break

        return PsyQObject(
            sections={
                names[i]: bytes(buf) for i, buf in content.items() if i in names
            },
            relocs={names[i]: rs for i, rs in relocs.items() if i in names},
            exports=exports,
            section_names=dict(names),
            truncated=truncated,
            stopped_at=stopped_at,
            unknown_kind=unknown_kind,
        )
