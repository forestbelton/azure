import argparse
import dataclasses
import glob
import pathlib
from typing import Optional

import psylib


@dataclasses.dataclass
class SymInfo:
    library: str
    module: str


SymInfoMap = dict[str, SymInfo]


def extract_lib(library_file: str, info: Optional[SymInfoMap] = None) -> SymInfoMap:
    info = info or {}
    lib_name = pathlib.Path(library_file).name.replace(".LIB", "").lower()
    with open(library_file, "rb") as f:
        lib = psylib.PsyQLibrary.decode(f)
    for module in lib.modules:
        for external in module.externals:
            if external.name in info:
                existing_lib = info[external.name].library
                existing_mod = info[external.name].module
                print(
                    f"warning: symbol '{external.name}' redefined in {lib_name}.{module.name}! (previously: {existing_lib}.{existing_mod})"
                )
                continue
            info[external.name] = SymInfo(library=lib_name, module=module.name)
    return info


def extract_dir(lib_dir: str) -> SymInfoMap:
    info: SymInfoMap = {}
    lib_dir_path = pathlib.Path(lib_dir).resolve()
    for lib_file in glob.glob("*.LIB", root_dir=lib_dir_path):
        info = extract_lib(lib_dir_path / lib_file, info)
    return info


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("psyq_libs_dir")
    args = parser.parse_args()
    info = extract_dir(args.psyq_libs_dir)
    print(info)


if __name__ == "__main__":
    main()
