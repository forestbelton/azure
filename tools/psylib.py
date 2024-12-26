import argparse
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


def add_modules(libraryfile: str, modules: list[str]) -> None:
    raise NotImplementedError()


def delete_modules(libraryfile: str, modules: list[str]) -> None:
    with open(libraryfile, "rb") as f:
        library = PsyQLibrary.decode(f)
    updated_library = PsyQLibrary(modules=[])
    deleted_module_names: list[str] = []
    for module in library.modules:
        if len(modules) > 0 and module.name.upper() not in modules:
            updated_library.modules.append(module)
            continue
        deleted_module_names.append(module.name)
    with open(libraryfile, "wb") as f:
        updated_library.encode(f)
    for module_name in deleted_module_names:
        print(f"Deleted module {module_name}")


def update_modules(libraryfile: str, modules: list[str]) -> None:
    raise NotImplementedError()


def extract_modules(libraryfile: str, modules: list[str]) -> None:
    with open(libraryfile, "rb") as f:
        library = PsyQLibrary.decode(f)
    for module in library.modules:
        if len(modules) > 0 and module.name.upper() not in modules:
            continue
        module_name = f"{module.name}.OBJ"
        with open(module_name, "wb") as f:
            f.write(module.data)
        print(f"Extracted object file {module_name}")


def list_modules(libraryfile: str, modules: list[str]) -> None:
    with open(libraryfile, "rb") as f:
        library = PsyQLibrary.decode(f)
    print("Module     Date     Time   Externals defined")
    for module in library.modules:
        if len(modules) > 0 and module.name.upper() not in modules:
            continue
        print(f"{module.name:<8} ", end="")
        # TODO: Figure out date/time encoding
        print(f"{'':<17} ", end="")
        first_on_line = True
        cur_external_line = ""
        external_lines: list[str] = []
        for external in module.externals:
            external_name = external.name
            if external.uninitialized:
                external_name = "*" + external_name
            added_len = len(external_name)
            assert added_len < 77 - 27
            if not first_on_line:
                added_len += 1
            if len(cur_external_line) + added_len > 77 - 27:
                external_lines.append(cur_external_line)
                cur_external_line = ""
                first_on_line = True
            if not first_on_line:
                cur_external_line = cur_external_line + " "
            cur_external_line = cur_external_line + external_name
            first_on_line = False
        if len(cur_external_line) != 0:
            external_lines.append(cur_external_line)
        for i, line in enumerate(external_lines):
            if i > 0:
                print(" " * 27, end="")
            print(line)


ALL_ACTIONS = [
    ("--add", add_modules),
    ("--delete", delete_modules),
    ("--update", update_modules),
    ("--extract", extract_modules),
    ("--list", list_modules),
]


def main() -> None:
    print("\nPsyLib version 4.20 😎\n")
    parser = argparse.ArgumentParser(allow_abbrev=False, add_help=False)
    flag_group = parser.add_mutually_exclusive_group(required=True)
    for flag, action in ALL_ACTIONS:
        flag_group.add_argument(flag, dest="action", action="store_const", const=action)
    parser.add_argument("libraryfile")
    parser.add_argument("modules", nargs="*")
    args = parser.parse_args()
    args.action(args.libraryfile, [module.upper() for module in args.modules])


if __name__ == "__main__":
    main()
