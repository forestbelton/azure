import dataclasses
import struct
from typing import BinaryIO, Optional


@dataclasses.dataclass
class External:
    name: str
    uninitialized: bool


@dataclasses.dataclass
class PsyQModule:
    name: str
    date: int
    externals: list[External]
    data: bytes

    @staticmethod
    def decode(file: BinaryIO) -> Optional["PsyQModule"]:
        chunk = file.read(20)
        if len(chunk) == 0:
            return None
        assert len(chunk) == 20
        header: tuple[bytes, int, int, int] = struct.unpack("<8sIII", chunk)
        name, date, link_offset, next_offset = header
        externals: list[External] = []
        (sz,) = struct.unpack("B", file.read(1))
        while sz > 0:
            external = file.read(sz)
            assert len(external) == sz
            uninitialized = False
            if external[0] == 0:
                uninitialized = True
                external = external[1:]
            externals.append(
                External(name=external.decode(), uninitialized=uninitialized)
            )
            (sz,) = struct.unpack("B", file.read(1))
        data = file.read(next_offset - link_offset)
        assert len(data) == next_offset - link_offset
        return PsyQModule(
            name=name.rstrip().decode(),
            date=date,
            externals=externals,
            data=data,
        )

    def encode(self, file: BinaryIO) -> None:
        # NB: Not sure where this is coming from lol!
        link_offset = 21 + len(self.externals)
        for external in self.externals:
            link_offset += len(external.name)
            if external.uninitialized:
                link_offset += 1
        next_offset = len(self.data) + link_offset
        file.write(
            struct.pack(
                "<8sIII",
                f"{self.name:<8}".encode(),
                self.date,
                link_offset,
                next_offset,
            )
        )
        for external in self.externals:
            external_name = external.name.encode()
            if external.uninitialized:
                external_name = b"\x00" + external_name
            file.write(bytes([len(external_name)]))
            file.write(external_name)
        file.write(b"\x00")
        file.write(self.data)


@dataclasses.dataclass
class PsyQLibrary:
    modules: list[PsyQModule]

    @staticmethod
    def decode(file: BinaryIO) -> "PsyQLibrary":
        magic, version = struct.unpack("3sB", file.read(4))
        assert magic == b"LIB"
        assert version == 1
        modules: list[PsyQModule] = []
        module = PsyQModule.decode(file)
        while module is not None:
            modules.append(module)
            module = PsyQModule.decode(file)
        return PsyQLibrary(modules=modules)

    def encode(self, file: BinaryIO) -> None:
        file.write(b"LIB\x01")
        for module in self.modules:
            module.encode(file)
