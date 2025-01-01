import argparse

from azure import psylib


def add_modules(libraryfile: str, modules: list[str]) -> None:
    raise NotImplementedError()


def delete_modules(libraryfile: str, modules: list[str]) -> None:
    with open(libraryfile, "rb") as f:
        library = psylib.PsyQLibrary.decode(f)
    updated_library = psylib.PsyQLibrary(modules=[])
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
        library = psylib.PsyQLibrary.decode(f)
    for module in library.modules:
        if len(modules) > 0 and module.name.upper() not in modules:
            continue
        module_name = f"{module.name}.OBJ"
        with open(module_name, "wb") as f:
            f.write(module.data)
        print(f"Extracted object file {module_name}")


def list_modules(libraryfile: str, modules: list[str]) -> None:
    with open(libraryfile, "rb") as f:
        library = psylib.PsyQLibrary.decode(f)
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
    ("--add", "add new module(s)", add_modules),
    ("--delete", "delete module(s)", delete_modules),
    ("--update", "update existing module(s)", update_modules),
    ("--extract", "extract module(s) to object files", extract_modules),
    ("--list", "list module(s)", list_modules),
]


def setup_parser(prog: str) -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog,
        allow_abbrev=False,
        add_help=False,
        description="""
            Recreation of PSYLIB.EXE in Python. The program is compatible with
            all PSY-Q libraries but has a modernized command-line interface.
        """.strip(),
    )
    flag_group = parser.add_mutually_exclusive_group(required=True)
    for flag, help, action in ALL_ACTIONS:
        flag_group.add_argument(
            flag, dest="action", action="store_const", const=action, help=help
        )
    parser.add_argument("libraryfile", help="path of library file")
    parser.add_argument(
        "modules", nargs="*", help="list of modules in library to operate over"
    )
    parser.set_defaults(main=main)
    return parser


def main(args: argparse.Namespace) -> None:
    args.action(args.libraryfile, [module.upper() for module in args.modules])


if __name__ == "__main__":
    parser = setup_parser("psylib")
    main(parser.parse_args())
